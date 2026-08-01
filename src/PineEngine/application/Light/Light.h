#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace PineEngine {
    struct PointLight {
        glm::vec3 translation;
        glm::vec3 radiantIntensity; // W/sr
        bool enableShadows = false;
        bool enableSSAO = false;
        bool enableSpecular = true;
    };

    struct DirectionalLight {
        glm::vec3 direction;
        glm::vec3 irradiance; // W/m2
        bool enableShadows = false;
        bool enableSSAO = false;
        bool enableSpecular = true;
    };
}
