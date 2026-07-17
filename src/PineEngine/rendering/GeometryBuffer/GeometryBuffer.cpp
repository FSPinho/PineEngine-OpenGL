#pragma once

#include "GeometryBuffer.h"


namespace PineEngine {
    GeometryBuffer::GeometryBuffer(
        RendererBackend &backend,
        std::vector<float> &&vertices_,
        std::vector<int> &&indices_
    ) : RendererComponent(backend), vertices(std::move(vertices_)), indices(std::move(indices_)) {
        this->geometryId = this->backend.createGeometry();
        this->verticesBufferId = this->backend.createDataBuffer();
        this->indicesBufferId = this->backend.createDataBuffer();
        this->backend.populateVecFloat3DataBuffer(
            this->verticesBufferId,
            this->vertices
        );
        this->backend.populateIndexDataBuffer(
            this->indicesBufferId,
            this->indices
        );
        this->backend.bindVecFloat3DataBufferToGeometry(
            this->geometryId,
            this->verticesBufferId,
            0 // First attribute
        );
        this->backend.bindIndexDataBufferToGeometry(
            this->geometryId,
            this->indicesBufferId
        );
        this->isGeometryLoaded = true;

        LOG_CONSTRUCTOR("GeometryBuffer");
    }

    GeometryBuffer::~GeometryBuffer() {
        LOG_DESTRUCTOR("GeometryBuffer");

        if (this->isGeometryLoaded) {
            this->backend.deleteDataBuffer(this->verticesBufferId);
            this->backend.deleteDataBuffer(this->indicesBufferId);
            this->backend.deleteGeometry(this->geometryId);
        }
    }

    void GeometryBuffer::process() {
        this->backend.prepareGeometryForRendering(this->geometryId);
        this->backend.drawTriangles(this->indices.size());
    }
}
