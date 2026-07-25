#pragma once

#include <PineEngine/rendering/RendererComponent/RendererComponent.h>
#include <PineEngine/util/GeometryLoader/GeometryLoader.h>
#include <PineEngine/util/Path/Path.h>
#include <PineEngine/util/Resource/Resource.h>
#include <vector>

namespace PineEngine {
    class GeometryBuffer : public Resource, public RendererComponent {
    public:
        explicit GeometryBuffer(const Path &path, const GeometryPreset &preset, RendererBackend &backend);
        explicit GeometryBuffer(const Path &path, RendererBackend &backend);
        ~GeometryBuffer() override;

        void performRendering() override;

        void enableWireframe();

        void calculateLightInfluence(const std::vector<float> &lightPositions);

    protected:
        void performLoad() override;
        void performUnload() override;

    private:
        GeometryLoader loader;

        std::vector<VertexData> verticesData;
        std::vector<uint32_t> indices;

        uint32_t geometryId = 0;
        uint32_t verticesBufferId = 0;
        uint32_t indicesBufferId = 0;
        bool isGeometryLoaded = false;
        uint32_t indicesCount = 0;

        bool wireFrameEnabled = false;

        bool hasCalculatedLightInfluence = false;

        void _validateGeometry();
        void _uploadGeometryToGPU();
        void _deleteGeometryFromGPU();
    };
} // namespace PineEngine
