#include "FrameBuffer.h"

namespace PineEngine {
    FrameBuffer::FrameBuffer(const Path &path, const RendererBackend &backend) : Resource(path), backend(backend) {
    }

    void FrameBuffer::resize(const uint32_t width_, const uint32_t height_) {
        if (this->width == width_ && this->height == height_) return;
        this->width = width_;
        this->height = height_;
        this->backend.allocateColorTexture(this->colorTextureId, this->width, this->height);
        this->backend.allocateDepthTexture(this->depthTextureId, this->width, this->height);
    }

    uint32_t FrameBuffer::getColorTextureId() {
        return this->colorTextureId;
    }

    uint32_t FrameBuffer::getDepthTextureId() {
        return this->depthTextureId;
    }

    void FrameBuffer::prepareForRendering() {
        this->backend.attachColorTextureToFrameBuffer(this->colorFrameBufferId, this->colorTextureId);
        this->backend.attachDepthTextureToFrameBuffer(this->depthFrameBufferId, this->depthTextureId);
        this->backend.prepareFrameBufferForRendering(this->colorFrameBufferId, this->width, this->height);
        this->backend.clearFrame();
    }

    void FrameBuffer::performLoad() {
        this->colorFrameBufferId = this->backend.createFrameBuffer();
        this->colorTextureId = this->backend.createTexture();
        this->backend.configureTextureFilterNearest(this->colorTextureId);
        this->backend.configureTextureClampToEdge(this->colorTextureId);

        this->depthFrameBufferId = this->backend.createDepthFrameBuffer();
        this->depthTextureId = this->backend.createTexture();
        this->backend.configureTextureFilterNearest(this->depthTextureId);

        this->isLoaded = true;
    }

    void FrameBuffer::performUnload() {
        if (this->isLoaded) {
            this->backend.deleteTexture(this->colorTextureId);
            this->backend.deleteTexture(this->depthTextureId);
            this->backend.deleteFrameBuffer(this->colorFrameBufferId);
            this->backend.deleteFrameBuffer(this->depthFrameBufferId);
        }
    }
}
