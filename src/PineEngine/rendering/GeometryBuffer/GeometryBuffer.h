#pragma once

#include <string>
#include <vector>
#include <PineEngine/rendering/RendererComponent/RendererComponent.h>


namespace PineEngine {
    struct VertexData {
        std::string name;
        std::vector<float> data;
        uint32_t dimensionality;
    };

    class GeometryBuffer : public RendererComponent {
    public:
        explicit GeometryBuffer(
            RendererBackend &backend,
            std::vector<VertexData> &&verticesData_,
            std::vector<uint32_t> &&indices_
        );
        ~GeometryBuffer() override;

    protected:
        void process() override;

    private:
        uint32_t geometryId = 0;
        uint32_t verticesBufferId = 0;
        uint32_t indicesBufferId = 0;
        bool isGeometryLoaded = false;
        std::vector<VertexData> verticesData;
        std::vector<uint32_t> indices;

        void _validateGeometry();
        void _loadGeometry();
        void _unloadGeometry();
    };
}
