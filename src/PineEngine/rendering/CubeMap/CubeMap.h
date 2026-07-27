#pragma once

#include <PineEngine/rendering/RendererBackend/RendererBackend.h>
#include <PineEngine/util/Resource/Resource.h>

namespace PineEngine {
    class CubeMap : public Resource {
    public:
        explicit CubeMap(const Path &path, const RendererBackend &backend);
        ~CubeMap() override;

    protected:
        void performLoad() override;
        void performUnload() override;

    private:
        RendererBackend backend;
        uint32_t textureId = 0;

        bool isLoaded = false;
    };
}
