#pragma once

#include <vector>
#include <PineEngine/rendering/RendererComponent/RendererComponent.h>


namespace PineEngine {
    class GeometryBuffer : public RendererComponent {
    public:
        explicit GeometryBuffer(
            RendererBackend &backend,
            std::vector<float> &&vertices_,
            std::vector<int> &&indices_
        );
        ~GeometryBuffer() override;

    protected:
        void process() override;

    private:
        uint32_t geometryId = 0;
        uint32_t verticesBufferId = 0;
        uint32_t indicesBufferId = 0;
        bool isGeometryLoaded = false;
        std::vector<float> vertices;
        std::vector<int> indices;
    };
}
