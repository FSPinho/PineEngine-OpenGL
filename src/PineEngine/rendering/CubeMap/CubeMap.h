#pragma once

#include <PineEngine/util/Resource/Resource.h>
#include <PineEngine/util/ResourceHandler/ResourceHandler.h>
#include <PineEngine/rendering/RendererBackend/RendererBackend.h>
#include <PineEngine/rendering/FrameBuffer/FrameBuffer.h>
#include <PineEngine/rendering/GeometryBuffer/GeometryBuffer.h>
#include <PineEngine/rendering/Texture/Texture.h>

namespace PineEngine {
    class CubeMap : public Resource {
    public:
        explicit CubeMap(const Path &path, const Path &sourcePath, const RendererBackend &backend);
        ~CubeMap() override;

        uint32_t getTextureId();

    protected:
        void performLoad() override;
        void performUnload() override;

    private:
        RendererBackend backend;

        ResourceHandler<Texture> cubeMapSourceTexture;
        ResourceHandler<FrameBuffer> cubeMapFrameBuffer;
        ResourceHandler<GeometryBuffer> cubeMapGeometry;
    };
}
