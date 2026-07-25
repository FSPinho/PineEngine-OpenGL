#pragma once

#include <PineEngine/util/Path/Path.h>
#include <vector>

namespace PineEngine {
    enum class GeometryPreset { NONE, BOX, SPHERE, QUAD };

    struct VertexData {
        std::string name;
        std::vector<float> data;
        uint32_t dimensionality;
    };

    class GeometryLoader {
    public:
        explicit GeometryLoader(const Path &path);
        explicit GeometryLoader(const GeometryPreset &preset);

        std::pair<std::vector<VertexData>, std::vector<uint32_t>> load();

    private:
        Path path;
        GeometryPreset preset;

        std::pair<std::vector<VertexData>, std::vector<uint32_t> > _loadFromFile();
        std::pair<std::vector<VertexData>, std::vector<uint32_t> > _loadQuad();
    };
} // namespace PineEngine
