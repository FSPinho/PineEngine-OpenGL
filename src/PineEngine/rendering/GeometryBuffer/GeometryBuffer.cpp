#include "GeometryBuffer.h"
#include <cassert>
#include <stdexcept>


namespace PineEngine {
    GeometryBuffer::GeometryBuffer(
        RendererBackend &backend,
        std::vector<VertexData> &&verticesData_,
        std::vector<uint32_t> &&indices_
    ) : RendererComponent(backend), verticesData(std::move(verticesData_)), indices(std::move(indices_)) {
        LOG_CONSTRUCTOR("GeometryBuffer");

        this->_validateGeometry();
        this->_loadGeometry();
    }

    GeometryBuffer::~GeometryBuffer() {
        LOG_DESTRUCTOR("GeometryBuffer");
        this->_unloadGeometry();
    }

    void GeometryBuffer::process() {
        this->backend.prepareGeometryForRendering(this->geometryId);
        this->backend.drawTriangles(this->indices.size());
    }

    void GeometryBuffer::_validateGeometry() {
        if (this->verticesData.size() == 0) {
            throw std::runtime_error("Vertices data must have at least one attribute!");
        }

        const uint32_t sizeRef = this->verticesData[0].data.size() / this->verticesData[0].dimensionality;
        if (sizeRef == 0) {
            throw std::runtime_error("Vertices data cannot be empty!");
        }

        for (const auto &vertexData: this->verticesData) {
            if (vertexData.data.size() % vertexData.dimensionality != 0) {
                throw std::runtime_error(FORMAT(
                    "The size of the vertex data \"{}\" should be divisible by {}!",
                    vertexData.name,
                    vertexData.dimensionality
                ));
            }

            if (sizeRef != vertexData.data.size() / vertexData.dimensionality) {
                throw std::runtime_error("The data size of each vertex attribute should be the same!");
            }
        }
    }

    void GeometryBuffer::_loadGeometry() {
        this->geometryId = this->backend.createGeometry();
        this->verticesBufferId = this->backend.createDataBuffer();
        this->indicesBufferId = this->backend.createDataBuffer();

        // Allocate buffer memory
        uint32_t combinedVerticesDataSize = 0;
        uint32_t combinedVerticesDataSizeInBytes = 0;
        uint32_t combinedDimensionality = 0;
        for (const auto vertexData: this->verticesData) {
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
        this->backend.populateDataBuffer(
            this->verticesBufferId,
            combinedData.data(),
            0, combinedVerticesDataSizeInBytes
        );

        // Bind each attribute's buffer with the geometry
        uint32_t offset = 0;
        for (uint32_t i = 0; i < this->verticesData.size(); i++) {
            this->backend.bindDataBufferToGeometry(
                this->geometryId,
                this->verticesBufferId,
                i,
                this->verticesData[i].dimensionality,
                combinedDimensionality,
                offset
            );
            offset += this->verticesData[i].dimensionality;
        }

        // Indices
        this->backend.allocateIndexDataBuffer(this->indicesBufferId, this->indices.size() * sizeof(this->indices[0]));
        this->backend.populateIndexDataBuffer(
            this->indicesBufferId,
            this->indices.data(),
            0, this->indices.size() * sizeof(this->indices[0])
        );
        this->backend.bindIndexDataBufferToGeometry(
            this->geometryId,
            this->indicesBufferId
        );
        this->isGeometryLoaded = true;
    }

    void GeometryBuffer::_unloadGeometry() {
        if (this->isGeometryLoaded) {
            this->backend.deleteDataBuffer(this->verticesBufferId);
            this->backend.deleteDataBuffer(this->indicesBufferId);
            this->backend.deleteGeometry(this->geometryId);
        }
    }
}
