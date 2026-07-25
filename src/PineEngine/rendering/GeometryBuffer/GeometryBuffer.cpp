#include "GeometryBuffer.h"

#include <PineEngine/util/OcclusionMap/OcclusionMap.h>
#include <PineEngine/util/Timer/Timer.h>
#include <stdexcept>
#include <numbers>
#include <string>

namespace PineEngine {
    GeometryBuffer::GeometryBuffer(const Path &path, const GeometryPreset &preset, RendererBackend &backend)
        : Resource(path), RendererComponent(backend), loader(preset) {
        LOG_CONSTRUCTOR("GeometryBuffer");
    }

    GeometryBuffer::GeometryBuffer(const Path &path, RendererBackend &backend) : Resource(path),
        RendererComponent(backend), loader(path) {
    }

    GeometryBuffer::~GeometryBuffer() {
        LOG_DESTRUCTOR("GeometryBuffer");
    }

    void GeometryBuffer::performRendering() {
        this->backend.prepareGeometryForRendering(this->geometryId);

        if (this->wireFrameEnabled) {
            this->backend.drawWireframe(this->indicesCount);
        } else {
            this->backend.drawTriangles(this->indicesCount);
        }
    }

    void GeometryBuffer::prepareForCompute() {
        this->backend.bindDataBufferToCompute(this->verticesBufferId, 0);
        this->backend.bindDataBufferToCompute(this->indicesBufferId, 1);
    }

    uint32_t GeometryBuffer::getVertexCount() {
        return this->verticesData[0].data.size() / this->verticesData[0].dimensionality;
    }

    uint32_t GeometryBuffer::getIndexCount() {
        return this->indicesCount / 3;
    }

    void GeometryBuffer::enableWireframe() {
        this->wireFrameEnabled = true;
    }

    void GeometryBuffer::calculateLightInfluence(const std::vector<float> &lightPositions) {
        if (this->hasCalculatedLightInfluence) return;

        Timer t;

        for (uint32_t lightIndex = 0; lightIndex < lightPositions.size(); lightIndex += 3) {
            LOG(FORMAT("Processing light {}/{}", (lightIndex / 3) + 1, lightPositions.size() / 3));

            const glm::vec3 lightPos(
                lightPositions[lightIndex],
                lightPositions[lightIndex + 1],
                lightPositions[lightIndex + 2]
            );
            OcclusionMap map(lightPos, std::numbers::pi / 128.0f);
            map.computeFrom(this->verticesData[0].data, this->indices);

            for (uint32_t vertexIndex = 0; vertexIndex < this->verticesData[0].data.size(); vertexIndex += 3) {
                auto flags = static_cast<uint32_t>(this->verticesData[2].data[vertexIndex / 3]);

                if (map.isOccluded(vertexIndex / 3, this->verticesData[0].data, this->indices)) {
                    flags &= ~(1 << (lightIndex / 3));
                } else {
                    flags |= 1 << (lightIndex / 3);
                }

                this->verticesData[2].data[vertexIndex / 3] = static_cast<float>(flags);
            }
        }

        const auto [elapsed, delta] = t.getNextTick();
        LOG(FORMAT("Processed all lights in {:.4f} seconds", elapsed));

        this->_deleteGeometryFromGPU();
        this->_uploadGeometryToGPU();

        this->hasCalculatedLightInfluence = true;
    }

    void GeometryBuffer::performLoad() {
        std::tie(this->verticesData, this->indices) = this->loader.load();
        this->verticesData.push_back({
            .name = "vertexInLightInfluence",
            .data = std::vector(this->verticesData[0].data.size(), 0.0f),
            .dimensionality = 4
        });

        this->_validateGeometry();
        this->_uploadGeometryToGPU();
    }

    void GeometryBuffer::performUnload() {
        this->_deleteGeometryFromGPU();
    }

    void GeometryBuffer::_validateGeometry() {
        if (this->verticesData.empty()) {
            throw std::runtime_error("Vertices data must have at least one attribute!");
        }

        const uint32_t sizeRef = this->verticesData[0].data.size() / this->verticesData[0].dimensionality;
        if (sizeRef == 0) {
            throw std::runtime_error("Vertices data cannot be empty!");
        }

        for (const auto &vertexData: this->verticesData) {
            if (vertexData.data.size() % vertexData.dimensionality != 0) {
                throw std::runtime_error(FORMAT("The size of the vertex data \"{}\" should be divisible by {}!",
                                                vertexData.name, vertexData.dimensionality));
            }

            if (sizeRef != vertexData.data.size() / vertexData.dimensionality) {
                throw std::runtime_error("The data size of each vertex attribute should be the same!");
            }
        }
    }

    void GeometryBuffer::_uploadGeometryToGPU() {
        this->geometryId = this->backend.createGeometry();
        this->verticesBufferId = this->backend.createDataBuffer();
        this->indicesBufferId = this->backend.createDataBuffer();

        // Allocate buffer memory
        uint32_t combinedVerticesDataSize = 0;
        uint32_t combinedVerticesDataSizeInBytes = 0;
        uint32_t combinedDimensionality = 0;
        for (const auto &vertexData: this->verticesData) {
            combinedVerticesDataSize += vertexData.data.size();
            combinedVerticesDataSizeInBytes += vertexData.data.size() * sizeof(vertexData.data[0]);
            combinedDimensionality += vertexData.dimensionality;
        }
        this->backend.allocateDataBuffer(this->verticesBufferId, combinedVerticesDataSizeInBytes);

        // Interleave data before populating the buffer
        std::vector<float> combinedData(combinedVerticesDataSize);
        uint32_t combinedDataCursor = 0;
        for (uint32_t i = 0; i < combinedVerticesDataSize / combinedDimensionality; i++) {
            for (const auto &vertexData: this->verticesData) {
                for (uint32_t di = 0; di < vertexData.dimensionality; di++) {
                    combinedData[combinedDataCursor] = vertexData.data[i * vertexData.dimensionality + di];
                    combinedDataCursor++;
                }
            }
        }

        // Populate the buffer
        this->backend.populateDataBuffer(this->verticesBufferId, combinedData.data(), 0,
                                         combinedVerticesDataSizeInBytes);

        // Bind each attribute's buffer with the geometry
        uint32_t offset = 0;
        for (uint32_t i = 0; i < this->verticesData.size(); i++) {
            this->backend.bindDataBufferToGeometry(
                this->geometryId, this->verticesBufferId, i,
                this->verticesData[i].dimensionality, combinedDimensionality,
                offset
            );
            offset += this->verticesData[i].dimensionality;
        }

        // Indices
        this->indicesCount = this->indices.size();
        this->backend.allocateIndexDataBuffer(this->indicesBufferId, this->indices.size() * sizeof(this->indices[0]));
        this->backend.populateIndexDataBuffer(this->indicesBufferId, this->indices.data(), 0,
                                              this->indices.size() * sizeof(this->indices[0]));
        this->backend.bindIndexDataBufferToGeometry(this->geometryId, this->indicesBufferId);
        this->isGeometryLoaded = true;
    }

    void GeometryBuffer::_deleteGeometryFromGPU() {
        if (this->isGeometryLoaded) {
            this->backend.deleteDataBuffer(this->verticesBufferId);
            this->backend.deleteDataBuffer(this->indicesBufferId);
            this->backend.deleteGeometry(this->geometryId);
        }
    }
} // namespace PineEngine
