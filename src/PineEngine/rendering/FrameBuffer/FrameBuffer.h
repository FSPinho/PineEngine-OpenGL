#pragma once

#include <PineEngine/util/Resource/Resource.h>
#include <PineEngine/rendering/RendererBackend/RendererBackend.h>

namespace PineEngine {
    class FrameBuffer : public Resource {
    public:
        explicit FrameBuffer(const Path &path, const RendererBackend &backend, bool multisampled = false, bool depth = true);
        ~FrameBuffer() override;

        void resize(uint32_t width_, uint32_t height_);
        uint32_t getColorTextureId();
        uint32_t getDepthTextureId();

        void clear();
        void clearColor();
        void clearDepth();
        void attachTextures();
        void prepareForRendering();

    protected:
        void performLoad() override;
        void performUnload() override;

    private:
        uint32_t frameBufferId = 0;
        uint32_t colorTextureId = 0;
        uint32_t depthTextureId = 0;

        bool multisampled;
        bool depth;
        bool isLoaded = false;

        RendererBackend backend;

        uint32_t width = 0;
        uint32_t height = 0;
    };
}
