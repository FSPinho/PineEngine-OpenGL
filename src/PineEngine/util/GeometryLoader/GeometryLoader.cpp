#include "GeometryLoader.h"

#include <stdexcept>

namespace PineEngine {
GeometryLoader::GeometryLoader(const Path &path) : path(path), preset(GeometryPreset::NONE) {}

GeometryLoader::GeometryLoader(const GeometryPreset &preset) : path(""), preset(preset) {}

std::vector<VertexData> GeometryLoader::getVerticesData() {
    if (this->preset == GeometryPreset::BOX) {
        return this->_getBoxVerticesData();
    }

    throw std::runtime_error("Failed to get vertex data!");
}

std::vector<uint32_t> GeometryLoader::getIndices() {
    if (this->preset == GeometryPreset::BOX) {
        return this->_getBoxIndices();
    }

    throw std::runtime_error("Failed to get indices!");
}

std::vector<VertexData> GeometryLoader::_getBoxVerticesData() {
    std::vector<VertexData> vertexData{{
                                           .name = "vertexInPosition",
                                           .data =
                                               {// Front (+Z)
                                                -0.5f,
                                                -0.5f,
                                                0.5f,
                                                0.5f,
                                                -0.5f,
                                                0.5f,
                                                0.5f,
                                                0.5f,
                                                0.5f,
                                                -0.5f,
                                                0.5f,
                                                0.5f,

                                                // Back (-Z)
                                                0.5f,
                                                -0.5f,
                                                -0.5f,
                                                -0.5f,
                                                -0.5f,
                                                -0.5f,
                                                -0.5f,
                                                0.5f,
                                                -0.5f,
                                                0.5f,
                                                0.5f,
                                                -0.5f,

                                                // Left (-X)
                                                -0.5f,
                                                -0.5f,
                                                -0.5f,
                                                -0.5f,
                                                -0.5f,
                                                0.5f,
                                                -0.5f,
                                                0.5f,
                                                0.5f,
                                                -0.5f,
                                                0.5f,
                                                -0.5f,

                                                // Right (+X)
                                                0.5f,
                                                -0.5f,
                                                0.5f,
                                                0.5f,
                                                -0.5f,
                                                -0.5f,
                                                0.5f,
                                                0.5f,
                                                -0.5f,
                                                0.5f,
                                                0.5f,
                                                0.5f,

                                                // Top (+Y)
                                                -0.5f,
                                                0.5f,
                                                0.5f,
                                                0.5f,
                                                0.5f,
                                                0.5f,
                                                0.5f,
                                                0.5f,
                                                -0.5f,
                                                -0.5f,
                                                0.5f,
                                                -0.5f,

                                                // Bottom (-Y)
                                                -0.5f,
                                                -0.5f,
                                                -0.5f,
                                                0.5f,
                                                -0.5f,
                                                -0.5f,
                                                0.5f,
                                                -0.5f,
                                                0.5f,
                                                -0.5f,
                                                -0.5f,
                                                0.5f},
                                           .dimensionality = 3,
                                       },
                                       {
                                           .name = "vertexInNormal",
                                           .data =
                                               {// Front (+Z)
                                                0,
                                                0,
                                                1,
                                                0,
                                                0,
                                                1,
                                                0,
                                                0,
                                                1,
                                                0,
                                                0,
                                                1,

                                                // Back (-Z)
                                                0,
                                                0,
                                                -1,
                                                0,
                                                0,
                                                -1,
                                                0,
                                                0,
                                                -1,
                                                0,
                                                0,
                                                -1,

                                                // Left (-X)
                                                -1,
                                                0,
                                                0,
                                                -1,
                                                0,
                                                0,
                                                -1,
                                                0,
                                                0,
                                                -1,
                                                0,
                                                0,

                                                // Right (+X)
                                                1,
                                                0,
                                                0,
                                                1,
                                                0,
                                                0,
                                                1,
                                                0,
                                                0,
                                                1,
                                                0,
                                                0,

                                                // Top (+Y)
                                                0,
                                                1,
                                                0,
                                                0,
                                                1,
                                                0,
                                                0,
                                                1,
                                                0,
                                                0,
                                                1,
                                                0,

                                                // Bottom (-Y)
                                                0,
                                                -1,
                                                0,
                                                0,
                                                -1,
                                                0,
                                                0,
                                                -1,
                                                0,
                                                0,
                                                -1,
                                                0},
                                           .dimensionality = 3,
                                       }};
    return vertexData;
}

std::vector<uint32_t> GeometryLoader::_getBoxIndices() {
    std::vector<uint32_t> indices{0,  1,  2,  2,  3,  0,  4,  5,  6,  6,  7,  4,  8,  9,  10, 10, 11, 8,
                                  12, 13, 14, 14, 15, 12, 16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20};
    return indices;
}
} // namespace PineEngine
