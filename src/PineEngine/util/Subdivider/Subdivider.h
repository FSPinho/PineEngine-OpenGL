#pragma once

#include <vector>

namespace PineEngine {
    class Subdivider {
    public:
        static std::pair<std::vector<float>, std::vector<uint32_t> > subdivide(
            const std::vector<float> &vertices,
            const std::vector<uint32_t> &indices,
            const uint32_t iterations
        ) {
            auto [vertices_, indices_] = subdivide(vertices, indices);
            for (uint32_t i = 0; i < iterations - 1; i++) {
                std::tie(vertices_, indices_) = subdivide(vertices_, indices_);
            }
            return {vertices_, indices_};
        }

        static std::pair<std::vector<float>, std::vector<uint32_t> > subdivide(
            const std::vector<float> &vertices,
            const std::vector<uint32_t> &indices
        ) {
            std::vector<float> newVertices(vertices.size());
            std::vector<uint32_t> newIndices;

            for (uint32_t i = 0; i < vertices.size(); i++) {
                newVertices[i] = vertices[i];
            }

            uint32_t vertexIndexOffset = newVertices.size() / 3;
            for (uint32_t i = 0; i < indices.size(); i += 3) {
                const auto v1i = indices[i];
                const auto v2i = indices[i + 1];
                const auto v3i = indices[i + 2];

                const auto x1 = vertices[v1i * 3];
                const auto y1 = vertices[v1i * 3 + 1];
                const auto z1 = vertices[v1i * 3 + 2];

                const auto x2 = vertices[v2i * 3];
                const auto y2 = vertices[v2i * 3 + 1];
                const auto z2 = vertices[v2i * 3 + 2];

                const auto x3 = vertices[v3i * 3];
                const auto y3 = vertices[v3i * 3 + 1];
                const auto z3 = vertices[v3i * 3 + 2];

                const auto xc = (x1 + x2 + x3) / 3.0f;
                const auto yc = (y1 + y2 + y3) / 3.0f;
                const auto zc = (z1 + z2 + z3) / 3.0f;

                newVertices.push_back(xc);
                newVertices.push_back(yc);
                newVertices.push_back(zc);

                newIndices.push_back(v1i);
                newIndices.push_back(v2i);
                newIndices.push_back(vertexIndexOffset);

                newIndices.push_back(v2i);
                newIndices.push_back(v3i);
                newIndices.push_back(vertexIndexOffset);

                newIndices.push_back(v3i);
                newIndices.push_back(v1i);
                newIndices.push_back(vertexIndexOffset);

                vertexIndexOffset += 1;
            }

            return {newVertices, newIndices};
        }
    };
}
