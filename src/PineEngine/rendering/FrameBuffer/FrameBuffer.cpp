#include "FrameBuffer.h"

namespace PineEngine {
    FrameBuffer::FrameBuffer(const Path &path, const RendererBackend &backend) : Resource(path), backend(backend) {
    }

    void FrameBuffer::resize(const uint32_t width_, const uint32_t height_) {
        if (this->width == width_ && this->height == height_) return;
        this->width = width_;
        this->height = height_;
        this->backend.allocateDepthFrameBuffer(this->depthFrameBufferId, this->width, this->height);
        this->backend.allocateColorTexture(this->colorTextureId, this->width, this->height);
    }

    uint32_t FrameBuffer::getColorTextureId() {
        return this->colorTextureId;
    }

    void FrameBuffer::prepareForRendering() {
        this->backend.attachColorTextureToFrameBuffer(this->frameBufferId, this->colorTextureId);
        this->backend.attachDepthTextureToFrameBuffer(this->frameBufferId);
        this->backend.prepareFrameBufferForRendering(this->frameBufferId, this->width, this->height);
        this->backend.clearFrame();
    }

    void FrameBuffer::performLoad() {
        this->frameBufferId = this->backend.createFrameBuffer();
        this->depthFrameBufferId = this->backend.createDepthFrameBuffer();
        this->colorTextureId = this->backend.createColorTexture();

        this->backend.configureTextureFilterNearest(this->colorTextureId);
        this->backend.configureTextureClampToEdge(this->colorTextureId);

        this->isLoaded = true;
    }

    void FrameBuffer::performUnload() {
        if (this->isLoaded) {
            this->backend.deleteTexture(this->colorTextureId);
            this->backend.deleteFrameBuffer(this->frameBufferId);
        }
    }
}
