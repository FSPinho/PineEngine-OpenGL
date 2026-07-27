#pragma once

#include <PineEngine/rendering/RendererBackend/RendererBackend.h>
#include <PineEngine/util/Resource/Resource.h>

namespace PineEngine {
    class Texture : public Resource {
    public:
        explicit Texture(const Path &path, const RendererBackend &backend);
        ~Texture() override;

        [[nodiscard]] uint32_t getTextureId() const;

    protected:
        void performLoad() override;
        void performUnload() override;

    private:
        RendererBackend backend;
        uint32_t textureId = 0;

        bool isLoaded = false;
    };
}
