#pragma once

#include <PineEngine/util/Path/Path.h>
#include <vector>

namespace PineEngine {
    struct VertexData {
        std::string name;
        std::vector<float> data;
        uint32_t dimensionality;
    };

    class GeometryLoader {
    public:
        explicit GeometryLoader(const Path &path);
        virtual ~GeometryLoader();

        virtual std::pair<std::vector<VertexData>, std::vector<uint32_t> > load();

    private:
        Path path;
        std::pair<std::vector<VertexData>, std::vector<uint32_t> > _loadFromFile();
    };
} // namespace PineEngine
