#include "Object.h"

namespace PineEngine {
    Object::Object() : id(SerialID::generate()) {
        LOG_CONSTRUCTOR(FORMAT("Object[{}]", this->id));
    }

    Object::~Object() {
        LOG_DESTRUCTOR(FORMAT("Object[{}]", this->id));
    }

    GeometryBuffer &Object::getGeometry() {
        return *(this->geometry);
    }

    void Object::performRendering(const Tick &tick, const Camera &camera,
                                  const std::vector<PointLight> &pointLights) {
        if (this->shaderSet) {
            this->shaderSet->setUniform("TIME", static_cast<float>(tick.elapsed));
            this->shaderSet->setUniform("MODEL_MATRIX", this->transform.getMatrix());
            this->shaderSet->setUniform("VIEW_MATRIX", camera.getViewMatrix());
            this->shaderSet->setUniform("VIEW_POSITION", camera.getTranslation());
            this->shaderSet->setUniform("PROJECTION_MATRIX", camera.getProjectionMatrix());

            this->shaderSet->setUniform(
                "POINT_LIGHTS_COUNT",
                std::vector{static_cast<uint32_t>(pointLights.size())}
            );

            for (uint32_t i = 0; i < pointLights.size(); i++) {
                std::string prefix = std::string("POINT_LIGHTS[") + std::to_string(i) + "].";
                this->shaderSet->setUniform(prefix + "translation", pointLights[i].translation);
                this->shaderSet->setUniform(prefix + "radiantIntensity", pointLights[i].radiantIntensity);
            }

            this->shaderSet->performRendering();
        }
        if (this->geometry) {
            std::vector<float> lightPositions;
            for (const auto &light: pointLights) {
                lightPositions.push_back(light.translation[0]);
                lightPositions.push_back(light.translation[1]);
                lightPositions.push_back(light.translation[2]);
            }
            this->geometry->calculateLightInfluence(lightPositions);
            this->geometry->performRendering();
        }
    }

    bool Object::operator==(const Object &other) const {
        return this->id == other.id;
    }

    Transform &Object::getTransform() {
        return this->transform;
    }
} // namespace PineEngine
