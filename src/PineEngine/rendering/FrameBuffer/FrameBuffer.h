#pragma once

#include <PineEngine/util/Resource/Resource.h>
#include <PineEngine/rendering/RendererBackend/RendererBackend.h>

namespace PineEngine {
    struct FrameBufferOptions {
        bool multisampled = false;
        bool depth = false;
        bool cubeMap = false;
    };

    class FrameBuffer : public Resource {
    public:
        explicit FrameBuffer(
            const Path &path,
            const RendererBackend &backend,
            const FrameBufferOptions &options
        );
        ~FrameBuffer() override;

        void resize(uint32_t width_, uint32_t height_);
        uint32_t getColorTextureId();
        uint32_t getDepthTextureId();

        void clear();
        void clearColor();
        void clearDepth();
        void attachTextures(uint32_t faceIndex = 0);
        void prepareForRendering();

    protected:
        void performLoad() override;
        void performUnload() override;

    private:
        uint32_t frameBufferId = 0;
        uint32_t colorTextureId = 0;
        uint32_t depthTextureId = 0;

        FrameBufferOptions options;
        bool isLoaded = false;

        RendererBackend backend;

        uint32_t width = 0;
        uint32_t height = 0;
    };
}
