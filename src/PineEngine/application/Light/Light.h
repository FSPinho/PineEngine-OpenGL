#pragma once

#include <vector>

namespace PineEngine {
    struct PointLight {
        std::vector<float> translation;
        std::vector<float> radiantIntensity;
    };
}
