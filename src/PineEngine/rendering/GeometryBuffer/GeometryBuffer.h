#pragma once

#include <PineEngine/rendering/RendererBackend/RendererBackend.h>
#include <PineEngine/util/GeometryLoader/GeometryLoader.h>
#include <PineEngine/util/Path/Path.h>
#include <PineEngine/util/Resource/Resource.h>
#include <vector>

namespace PineEngine {
    class GeometryBuffer : public Resource {
    public:
        static Path QUAD;
        static Path CUBE;
        static Path SPHERE;

        explicit GeometryBuffer(const Path &path, const RendererBackend &backend);
        ~GeometryBuffer() override;

        void prepareForCompute();
        void performRendering();

        [[nodiscard]] uint32_t getVertexCount() const;
        [[nodiscard]] uint32_t getIndexCount() const;
        [[nodiscard]] RendererBackend &getRendererBackend();

        void enableWireframe();

        void calculateLightInfluence(const std::vector<float> &lightPositions);

    protected:
        void performLoad() override;
        void performUnload() override;

    private:
        RendererBackend backend;
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
