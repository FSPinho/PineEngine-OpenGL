#include "VolumeBuffer.h"

namespace PineEngine {
    VolumeBuffer::VolumeBuffer(const Path &path, const uint32_t resolution, const RendererBackend &backend)
        : Resource(path), resolution(resolution), backend(backend) {
        LOG_CONSTRUCTOR(FORMAT("VolumeBuffer[{}]", this->resolution));
    }

    VolumeBuffer::~VolumeBuffer() {
        LOG_DESTRUCTOR(FORMAT("VolumeBuffer[{}]", this->resolution));
    }

    uint32_t VolumeBuffer::getResolution() const {
        return this->resolution;
    }

    void VolumeBuffer::prepareForCompute(uint32_t bufferAttributeIndex) {
        this->backend.bindDataBufferToShaderAccess(this->bufferId, bufferAttributeIndex);
    }

    void VolumeBuffer::performLoad() {
        this->bufferId = this->backend.createDataBuffer();
        this->backend.allocateDataBuffer(
            this->bufferId,
            this->resolution * this->resolution * this->resolution * sizeof(float)
        );
        this->isLoaded = true;
    }

    void VolumeBuffer::performUnload() {
        if (!this->isLoaded) {
            this->backend.deleteDataBuffer(this->bufferId);
        }
    }
}
