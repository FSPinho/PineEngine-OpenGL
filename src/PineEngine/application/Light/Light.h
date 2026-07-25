#pragma once

#include <vector>

namespace PineEngine {
    struct PointLight {
        std::vector<float> translation;
        std::vector<float> radiantIntensity; // W/sr
    };

    struct DirectionalLight {
        std::vector<float> direction;
        std::vector<float> irradiance; // W/m2
    };
}
