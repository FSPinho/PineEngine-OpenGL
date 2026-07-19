#pragma once

#include <PineEngine/util/Path/Path.h>
#include <vector>

namespace PineEngine {
enum class GeometryPreset { NONE, BOX };

struct VertexData {
    std::string name;
    std::vector<float> data;
    uint32_t dimensionality;
};

class GeometryLoader {

  public:
    explicit GeometryLoader(const Path &path);
    explicit GeometryLoader(const GeometryPreset &preset);

    std::vector<VertexData> getVerticesData();
    std::vector<uint32_t> getIndices();

  private:
    Path path;
    GeometryPreset preset;

    std::vector<VertexData> _getBoxVerticesData();
    std::vector<uint32_t> _getBoxIndices();
};
} // namespace PineEngine
