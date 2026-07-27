#include "FrameBuffer.h"

namespace PineEngine {
    FrameBuffer::FrameBuffer(
        const Path &path,
        const RendererBackend &backend,
        const FrameBufferOptions &options
    )
        : Resource(path), backend(backend), options(options) {
        LOG_CONSTRUCTOR(FORMAT("FrameBuffer[{}]", this->getPath().asString()));
    }

    FrameBuffer::~FrameBuffer() {
        LOG_DESTRUCTOR(FORMAT("FrameBuffer[{}]", this->getPath().asString()));
    }

    void FrameBuffer::resize(const uint32_t width_, const uint32_t height_) {
        if (this->width == width_ && this->height == height_) return;
        this->width = width_;
        this->height = height_;
        this->backend.allocateColorTexture(this->colorTextureId, this->width, this->height, this->options.multisampled);
        if (this->options.depth) {
            this->backend.allocateDepthTexture(this->depthTextureId, this->width, this->height, this->options.multisampled);
        }
    }

    uint32_t FrameBuffer::getColorTextureId() {
        return this->colorTextureId;
    }

    uint32_t FrameBuffer::getDepthTextureId() {
        return this->depthTextureId;
    }

    void FrameBuffer::clear() {
        this->backend.clearFrame(this->frameBufferId);
    }

    void FrameBuffer::clearColor() {
        this->backend.clearColor(this->frameBufferId);
    }

    void FrameBuffer::clearDepth() {
        this->backend.clearDepth(this->frameBufferId);
    }

    void FrameBuffer::attachTextures(const uint32_t faceIndex) {
        if (this->options.cubeMap) {
            this->backend.attachCubeMapTextureToFrameBuffer(this->frameBufferId, this->colorTextureId, faceIndex);
        } else {
            this->backend.attachColorTextureToFrameBuffer(this->frameBufferId, this->colorTextureId, this->options.multisampled);
        }

        if (this->options.depth) {
            this->backend.attachDepthTextureToFrameBuffer(this->frameBufferId, this->depthTextureId, this->options.multisampled);
        }
    }

    void FrameBuffer::prepareForRendering() {
        this->backend.prepareFrameBufferForRendering(this->frameBufferId, this->width, this->height);
    }

    void FrameBuffer::performLoad() {
        this->frameBufferId = this->backend.createFrameBuffer();

        this->colorTextureId = this->backend.createTexture();
        if (!this->options.multisampled) {
            this->backend.configureTextureFilterNearest(this->colorTextureId);
            this->backend.configureTextureClampToEdge(this->colorTextureId);
        }

        if (this->options.depth) {
            this->depthTextureId = this->backend.createTexture();
            if (!this->options.multisampled) {
                this->backend.configureTextureFilterNearest(this->depthTextureId);
                this->backend.configureTextureClampToEdge(this->depthTextureId);
            }
        }

        this->isLoaded = true;
    }

    void FrameBuffer::performUnload() {
        if (this->isLoaded) {
            this->backend.deleteTexture(this->colorTextureId);
            if (this->options.depth) {
                this->backend.deleteTexture(this->depthTextureId);
            }
            this->backend.deleteFrameBuffer(this->frameBufferId);
        }
    }
}
