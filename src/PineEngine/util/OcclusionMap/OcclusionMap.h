#pragma once

#include <utility>
#include <vector>
#include <unordered_map>
#include <ranges>
#include <glm/glm.hpp>

namespace PineEngine {
    class OcclusionMap {
    public:
        explicit OcclusionMap(const glm::vec3 &targetPoint, const float granularity)
            : targetPoint(targetPoint), granularity(granularity) {
            this->dimensionSize = static_cast<int64_t>(1.0f / this->granularity);
        }

        bool isOccluded(
            const uint32_t verticeIndex,
            const std::vector<float> &vertices,
            const std::vector<uint32_t> &indices
        ) {
            const glm::vec3 v(
                vertices[verticeIndex * 3],
                vertices[verticeIndex * 3 + 1],
                vertices[verticeIndex * 3 + 2]
            );

            const auto p = this->targetPoint - v;
            const auto pn = glm::normalize(p);

            const auto [key, distance] = this->_getVerticeInfo(v);
            const std::vector<uint32_t> &faces = this->verticeIndexToFaceIndexMap[verticeIndex];
            const auto nearbyFaceIndices = this->faceMap[key];

            for (const auto &faceIndex: nearbyFaceIndices | std::views::keys) {
                if (this->faceDistanceMap[faceIndex] > distance) continue;
                if (std::ranges::find(faces, faceIndex) != faces.end()) continue;

                const auto v1i = indices[faceIndex * 3];
                const auto v2i = indices[faceIndex * 3 + 1];
                const auto v3i = indices[faceIndex * 3 + 2];

                const auto p1 = this->targetPoint - glm::vec3(vertices[v1i * 3],
                                                              vertices[v1i * 3 + 1],
                                                              vertices[v1i * 3 + 2]);
                const auto p2 = this->targetPoint - glm::vec3(vertices[v2i * 3],
                                                              vertices[v2i * 3 + 1],
                                                              vertices[v2i * 3 + 2]);
                const auto p3 = this->targetPoint - glm::vec3(vertices[v3i * 3],
                                                              vertices[v3i * 3 + 1],
                                                              vertices[v3i * 3 + 2]);

                const auto p1n = glm::normalize(p1);
                const auto p2n = glm::normalize(p2);
                const auto p3n = glm::normalize(p3);

                const auto p1n_n = glm::normalize(p1n - pn);
                const auto p2n_n = glm::normalize(p2n - pn);
                const auto p3n_n = glm::normalize(p3n - pn);

                const auto dot1 = glm::dot(p1n_n, p2n_n);
                const auto dot2 = glm::dot(p2n_n, p3n_n);
                const auto dot3 = glm::dot(p3n_n, p1n_n);

                const uint32_t sum = (dot1 < 0) + (dot2 < 0) + (dot3 < 0);

                if (sum >= 2) {
                    return true;
                }
            }

            return false;
        }

        void computeFrom(
            const std::vector<float> &vertices,
            const std::vector<uint32_t> &indices
        ) {
            this->faceMap.clear();
            this->faceDistanceMap.clear();

            for (uint32_t i = 0; i < indices.size(); i += 3) {
                const auto faceIndex = i / 3;

                const glm::vec3 v1(
                    vertices[indices[i] * 3],
                    vertices[indices[i] * 3 + 1],
                    vertices[indices[i] * 3 + 2]
                );
                const glm::vec3 v2(
                    vertices[indices[i + 1] * 3],
                    vertices[indices[i + 1] * 3 + 1],
                    vertices[indices[i + 1] * 3 + 2]
                );
                const glm::vec3 v3(
                    vertices[indices[i + 2] * 3],
                    vertices[indices[i + 2] * 3 + 1],
                    vertices[indices[i + 2] * 3 + 2]
                );

                const auto n = glm::normalize(glm::cross(v3 - v2, v2 - v1));
                if (glm::dot(n, glm::normalize(this->targetPoint - v1)) < 0.0f) continue;

                this->_processFace(faceIndex, v1, v2, v3);
            }
        }

    private:
        glm::vec3 targetPoint;
        float granularity;
        int64_t dimensionSize;

        std::unordered_map<uint32_t, float> faceDistanceMap;
        std::unordered_map<int64_t, std::unordered_map<uint32_t, bool> > faceMap;
        std::unordered_map<uint32_t, std::vector<uint32_t> > verticeIndexToFaceIndexMap;

        void _processFace(const uint32_t &faceIndex, const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3) {
            const auto v1_ = this->targetPoint - v1;
            const auto v2_ = this->targetPoint - v2;
            const auto v3_ = this->targetPoint - v3;

            const auto v1n = glm::normalize(v1_);
            const auto v2n = glm::normalize(v2_);
            const auto v3n = glm::normalize(v3_);

            const auto arc1A = static_cast<int64_t>(v1n.x / this->granularity);
            const auto arc2A = static_cast<int64_t>(v2n.x / this->granularity);
            const auto arc3A = static_cast<int64_t>(v3n.x / this->granularity);
            const auto arcAStart = this->_fastMin(arc1A, arc2A, arc3A);
            const auto arcAEnd = this->_fastMax(arc1A, arc2A, arc3A);

            const auto arc1B = static_cast<int64_t>(v1n.y / this->granularity);
            const auto arc2B = static_cast<int64_t>(v2n.y / this->granularity);
            const auto arc3B = static_cast<int64_t>(v3n.y / this->granularity);
            const auto arcBStart = this->_fastMin(arc1B, arc2B, arc3B);
            const auto arcBEnd = this->_fastMax(arc1B, arc2B, arc3B);

            const auto arc1C = static_cast<int64_t>(v1n.z / this->granularity);
            const auto arc2C = static_cast<int64_t>(v2n.z / this->granularity);
            const auto arc3C = static_cast<int64_t>(v3n.z / this->granularity);
            const auto arcCStart = this->_fastMin(arc1C, arc2C, arc3C);
            const auto arcCEnd = this->_fastMax(arc1C, arc2C, arc3C);

            for (int64_t ai = arcAStart; ai <= arcAEnd; ai++) {
                for (int64_t bi = arcBStart; bi <= arcBEnd; bi++) {
                    for (int64_t ci = arcCStart; ci <= arcCEnd; ci++) {
                        const auto key = ai * dimensionSize * dimensionSize + bi * dimensionSize + ci;
                        this->faceMap[key].emplace(faceIndex, true);
                    }
                }
            }

            this->faceDistanceMap[faceIndex] = glm::length(v1_ + v2_ + v3_) / 3.0f;
        }

        std::pair<int64_t, float> _getVerticeInfo(const glm::vec3 &v) {
            const auto v_ = this->targetPoint - v;
            const auto vn = glm::normalize(v_);

            const auto arc1 = static_cast<int64_t>(vn.x / this->granularity);
            const auto arc2 = static_cast<int64_t>(vn.y / this->granularity);
            const auto arc3 = static_cast<int64_t>(vn.z / this->granularity);
            const auto key = arc1 * dimensionSize * dimensionSize + arc2 * dimensionSize + arc3;
            const auto distance = glm::length(v_);

            return {key, distance};
        }

        int64_t _fastMax(const int64_t &a, const int64_t &b, const int64_t &c) {
            return std::max(a, std::max(b, c));
        }

        int64_t _fastMin(const int64_t &a, const int64_t &b, const int64_t &c) {
            return std::min(a, std::min(b, c));
        }
    };
}
