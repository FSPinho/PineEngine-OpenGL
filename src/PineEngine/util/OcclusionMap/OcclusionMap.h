#pragma once

#include <cmath>
#include <utility>
#include <vector>
#include <unordered_map>
#include <numbers>
#include <ranges>
#include <glm/glm.hpp>
#include <PineEngine/util/Log/Log.h>

namespace PineEngine {
    class OcclusionMap {
    public:
        explicit OcclusionMap(const glm::vec3 &targetPoint, const float granularity)
            : targetPoint(targetPoint), granularity(granularity) {
            this->dimensionSize = static_cast<uint64_t>(std::numbers::pi / this->granularity);
        }

        bool isOccluded(
            const uint32_t verticeIndex,
            const std::vector<float> &vertices,
            const std::vector<uint32_t> &indices
        ) {
            const auto pTarget = glm::vec3(this->targetPoint[0], this->targetPoint[1], this->targetPoint[2]);

            const glm::vec3 v(
                vertices[verticeIndex * 3],
                vertices[verticeIndex * 3 + 1],
                vertices[verticeIndex * 3 + 2]
            );

            const auto p = pTarget - v;
            const auto pn = glm::normalize(p);

            const auto [key, distance] = this->_getVerticeInfo(v);
            const std::vector<uint32_t> &faces = this->verticeIndexToFaceIndexMap[verticeIndex];
            const auto nearbyFaceIndices = this->faceMap[key];

            for (const auto &faceIndex: nearbyFaceIndices | std::views::keys) {
                if (std::ranges::find(faces, faceIndex) != faces.end()) continue;
                if (this->faceDistanceMap[faceIndex] > distance) continue;

                const auto v1i = indices[faceIndex * 3];
                const auto v2i = indices[faceIndex * 3 + 1];
                const auto v3i = indices[faceIndex * 3 + 2];

                const auto p1 = pTarget - glm::vec3(vertices[v1i * 3], vertices[v1i * 3 + 1], vertices[v1i * 3 + 2]);
                const auto p2 = pTarget - glm::vec3(vertices[v2i * 3], vertices[v2i * 3 + 1], vertices[v2i * 3 + 2]);
                const auto p3 = pTarget - glm::vec3(vertices[v3i * 3], vertices[v3i * 3 + 1], vertices[v3i * 3 + 2]);

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

        void computeFrom(const std::vector<float> &vertices, const std::vector<uint32_t> &indices) {
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
                if (glm::dot(n, this->targetPoint - v1) < 0.0f) continue;
                
                this->_processFace(faceIndex, v1, v2, v3);
            }
        }

    private:
        glm::vec3 targetPoint;
        float granularity;
        uint64_t dimensionSize;

        std::unordered_map<uint32_t, float> faceDistanceMap;
        std::unordered_map<uint64_t, std::unordered_map<uint32_t, bool> > faceMap;
        std::unordered_map<uint32_t, std::vector<uint32_t> > verticeIndexToFaceIndexMap;

        void _processFace(const uint32_t &faceIndex, const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3) {
            const auto v1_ = this->targetPoint - v1;
            const auto v2_ = this->targetPoint - v2;
            const auto v3_ = this->targetPoint - v3;

            const auto arc1A = static_cast<uint64_t>(this->safeAtan2(v1_.y, v1_.x) / this->granularity);
            const auto arc2A = static_cast<uint64_t>(this->safeAtan2(v2_.y, v2_.x) / this->granularity);
            const auto arc3A = static_cast<uint64_t>(this->safeAtan2(v3_.y, v3_.x) / this->granularity);
            const auto arcAStart = std::min({arc1A, arc2A, arc3A});
            const auto arcAEnd = std::max({arc1A, arc2A, arc3A});

            const auto arc1B = static_cast<uint64_t>(this->safeAtan2(v1_.z, v1_.y) / this->granularity);
            const auto arc2B = static_cast<uint64_t>(this->safeAtan2(v2_.z, v2_.y) / this->granularity);
            const auto arc3B = static_cast<uint64_t>(this->safeAtan2(v3_.z, v3_.y) / this->granularity);
            const auto arcBStart = std::min({arc1B, arc2B, arc3B});
            const auto arcBEnd = std::max({arc1B, arc2B, arc3B});

            const auto arc1C = static_cast<uint64_t>(this->safeAtan2(v1_.x, v1_.z) / this->granularity);
            const auto arc2C = static_cast<uint64_t>(this->safeAtan2(v2_.x, v2_.z) / this->granularity);
            const auto arc3C = static_cast<uint64_t>(this->safeAtan2(v3_.x, v3_.z) / this->granularity);
            const auto arcCStart = std::min({arc1C, arc2C, arc3C});
            const auto arcCEnd = std::max({arc1C, arc2C, arc3C});

            for (uint64_t ai = arcAStart; ai <= arcAEnd; ai++) {
                for (uint64_t bi = arcBStart; bi <= arcBEnd; bi++) {
                    for (uint64_t ci = arcCStart; ci <= arcCEnd; ci++) {
                        const auto key = ai * dimensionSize * dimensionSize + bi * dimensionSize + ci;
                        this->faceMap[key].emplace(faceIndex, true);
                    }
                }
            }

            this->faceDistanceMap[faceIndex] = (glm::length(v1_) + glm::length(v2_) + glm::length(v3_)) / 3.0f;
        }

        std::pair<uint64_t, float> _getVerticeInfo(const glm::vec3 &v) {
            const auto v_ = this->targetPoint - v;

            const auto arc1 = static_cast<uint64_t>(this->safeAtan2(v_.y, v_.x) / this->granularity);
            const auto arc2 = static_cast<uint64_t>(this->safeAtan2(v_.z, v_.y) / this->granularity);
            const auto arc3 = static_cast<uint64_t>(this->safeAtan2(v_.x, v_.z) / this->granularity);
            const auto key = arc1 * dimensionSize * dimensionSize + arc2 * dimensionSize + arc3;
            const auto distance = glm::length(v_);

            return {key, distance};
        }

        float safeAtan2(const float x, const float y) {
            auto theta = std::atan2(y, x);
            if (theta < 0) {
                theta += std::numbers::pi * 2.0f;
            }
            return theta;
        }
    };
}
