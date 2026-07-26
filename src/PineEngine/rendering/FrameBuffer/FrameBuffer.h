#pragma once

#include <PineEngine/util/Resource/Resource.h>
#include <PineEngine/rendering/RendererBackend/RendererBackend.h>

namespace PineEngine {
    class FrameBuffer : public Resource {
    public:
        explicit FrameBuffer(const Path &path, const RendererBackend &backend);

        void resize(uint32_t width_, uint32_t height_);
        uint32_t getColorTextureId();
        uint32_t getDepthTextureId();

        void prepareForRendering();

    protected:
        void performLoad() override;
        void performUnload() override;

    private:
        uint32_t colorFrameBufferId = 0;
        uint32_t colorTextureId = 0;

        uint32_t depthFrameBufferId = 0;
        uint32_t depthTextureId = 0;

        bool isLoaded = false;

        RendererBackend backend;

        uint32_t width = 0;
        uint32_t height = 0;
    };
}
