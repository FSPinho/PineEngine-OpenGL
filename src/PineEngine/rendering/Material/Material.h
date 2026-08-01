#pragma once

#include <glm/glm.hpp>

namespace PineEngine {
    struct Material {
        glm::vec3 albedo = glm::vec3(1.0f, 1.0f, 1.0f);
        float roughness = 0.5f;
        glm::vec3 reflectance = glm::vec3(0.4f);
        float metallic = 0.0f;
        float transmission = 0.0f;
        glm::vec3 transmissionTint = glm::vec3(1.0f, 1.0f, 1.0f);

        static Material GOLD() {
            return {
                .roughness = 0.3f,
                .reflectance = {1.0f, 0.766f, 0.336f},
                .metallic = 1.0f
            };
        }
    };
}