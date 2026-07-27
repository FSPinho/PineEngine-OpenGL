#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace PineEngine {
    struct PointLight {
        glm::vec3 translation;
        glm::vec3 radiantIntensity; // W/sr

        [[nodiscard]] std::vector<float> getTranslationAsArray() const {
            return {this->translation.x, this->translation.y, this->translation.z};
        }

        [[nodiscard]] std::vector<float> getRadiantIntensityAsArray() const {
            return {this->radiantIntensity.x, this->radiantIntensity.y, this->radiantIntensity.z};
        }
    };

    struct DirectionalLight {
        glm::vec3 direction;
        glm::vec3 irradiance; // W/m2

        [[nodiscard]] std::vector<float> getDirectionAsArray() const {
            return {this->direction.x, this->direction.y, this->direction.z};
        }

        [[nodiscard]] std::vector<float> getIrradianceAsArray() const {
            return {this->irradiance.x, this->irradiance.y, this->irradiance.z};
        }
    };
}
