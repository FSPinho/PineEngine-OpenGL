#pragma once

#include <vector>

namespace PineEngine {
    class Terrain {
    public:
        static std::pair<std::vector<float>, std::vector<uint32_t> > decimate(
            const std::vector<float> &vertices,
            const std::vector<uint32_t> &indices
        ) {
            std::vector<float> newVertices(vertices.size());
            std::vector<uint32_t> newIndices(indices.size());

            for (uint32_t i = 0; i < vertices.size(); i++) {
                newVertices[i] = vertices[i];
            }

            for (uint32_t i = 0; i < indices.size(); i += 3) {
                newIndices[i] = indices[i];
            }

            return {newVertices, newIndices};
        }
    };
}
