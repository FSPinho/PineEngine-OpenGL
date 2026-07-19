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
            {
                // Bottom vertices
                -0.5f,
                -0.5f,
                -0.5f,
                0.5f,
                -0.5f,
                -0.5f,
                0.5f,
                0.5f,
                -0.5f,
                -0.5f,
                0.5f,
                -0.5f,

                // Top vertices
                -0.5f,
                0.5f,
                0.5f,
                0.5f,
                0.5f,
                0.5f,
                0.5f,
                -0.5f,
                0.5f,
                -0.5f,
                -0.5f,
                0.5f,
            },
        .dimensionality = 4,
    }};
    return vertexData;
}

std::vector<uint32_t> GeometryLoader::_getBoxIndices() {
    std::vector<uint32_t> indices{
        // Bottom face
        0,
        1,
        2,
        3,

        // Left face
        0,
        3,
        4,
        7,

        // Right face
        5,
        2,
        1,
        6,

        // Front face
        4,
        3,
        2,
        5,

        // Back face
        0,
        7,
        6,
        1,

        // Top face
        4,
        5,
        6,
        7,
    };
    return indices;
}
} // namespace PineEngine
