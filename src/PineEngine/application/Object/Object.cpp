#include "Object.h"

#include <numbers>

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

    void Object::performShadowMapPass(const Tick &tick, const Camera &lightCamera) {
        if (this->shadowMapShader) {
            this->shadowMapShader->setUniform("TIME", static_cast<float>(tick.elapsed));
            this->shadowMapShader->setUniform("MODEL_MATRIX", this->transform.getMatrix());
            this->shadowMapShader->setUniform("VIEW_MATRIX", lightCamera.getViewMatrix());
            this->shadowMapShader->setUniform("PROJECTION_MATRIX", lightCamera.getProjectionMatrix());
            this->shadowMapShader->prepareForRendering();
        }
        if (this->geometry) {
            this->geometry->performRendering();
        }
    }

    void Object::performColorPass(
        const Tick &tick,
        const Camera &camera,
        const Camera &lightCamera,
        const DirectionalLight *directionalLight,
        const PointLight *pointLight,
        const uint32_t &shadowMapTextureId
    ) {
        if (this->colorShader) {
            this->colorShader->setUniform("TIME", static_cast<float>(tick.elapsed));
            this->colorShader->setUniform("MODEL_MATRIX", this->transform.getMatrix());
            this->colorShader->setUniform("VIEW_MATRIX", camera.getViewMatrix());
            this->colorShader->setUniform("VIEW_MATRIX_INVERSE", camera.getViewMatrixInverse());
            this->colorShader->setUniform("VIEW_POSITION", camera.getTranslationAsArray());
            this->colorShader->setUniform("PROJECTION_MATRIX", camera.getProjectionMatrix());
            this->colorShader->setUniform("PROJECTION_MATRIX_INVERSE", camera.getProjectionMatrixInverse());
            this->colorShader->setUniform("LIGHT_VIEW_MATRIX", lightCamera.getViewMatrix());
            this->colorShader->setUniform("LIGHT_PROJECTION_MATRIX", lightCamera.getProjectionMatrix());

            this->colorShader->setUniformTexture("SHADOW_MAP", shadowMapTextureId);

            this->colorShader->setUniform("DIRECTIONAL_LIGHT.enabled", std::vector{directionalLight == nullptr ? 0u : 1u});
            this->colorShader->setUniform("POINT_LIGHT.enabled", std::vector{pointLight == nullptr ? 0u : 1u});

            if (directionalLight != nullptr) {
                this->colorShader->setUniform("DIRECTIONAL_LIGHT.direction", directionalLight->getDirectionAsArray());
                this->colorShader->setUniform("DIRECTIONAL_LIGHT.irradiance", directionalLight->getIrradianceAsArray());
                this->colorShader->setUniform("DIRECTIONAL_LIGHT.enableShadows", std::vector{static_cast<uint32_t>(directionalLight->enableShadows)});
                this->colorShader->setUniform("DIRECTIONAL_LIGHT.enableSSAO", std::vector{static_cast<uint32_t>(directionalLight->enableSSAO)});
            } else if (pointLight != nullptr) {
                this->colorShader->setUniform("POINT_LIGHT.translation", pointLight->getTranslationAsArray());
                this->colorShader->setUniform("POINT_LIGHT.radiantIntensity", pointLight->getRadiantIntensityAsArray());
                this->colorShader->setUniform("POINT_LIGHT.enableShadows", std::vector{static_cast<uint32_t>(pointLight->enableShadows)});
                this->colorShader->setUniform("POINT_LIGHT.enableSSAO", std::vector{static_cast<uint32_t>(pointLight->enableSSAO)});
            }

            this->colorShader->prepareForRendering();
        }
        if (this->geometry) {
            this->geometry->performRendering();
        }
    }

    void Object::performAddColorPass(const uint32_t colorTextureId, const bool multisampled) {
        if (this->colorShader) {
            this->colorShader->setUniformTexture("COLOR", colorTextureId, multisampled);
            this->colorShader->prepareForRendering();
        }
        if (this->geometry) {
            this->geometry->performRendering();
        }
    }

    void Object::performPostColorPass(
        const uint32_t &colorTextureId,
        const std::vector<DirectionalLight> &directionalLights,
        const std::vector<PointLight> &pointLights,
        const bool multisampled
    ) {
        float lightIntensity = 0.0f;
        for (const auto &light: directionalLights) {
            lightIntensity += glm::length(light.irradiance);
        }
        for (const auto &light: pointLights) {
            lightIntensity = glm::length(light.radiantIntensity) / std::pow(glm::length(light.translation), 2.0f);
        }
        const float exposure = 1.0f / (lightIntensity / static_cast<float>(std::numbers::pi));

        if (this->colorShader) {
            this->colorShader->setUniformTexture("COLOR", colorTextureId, multisampled);
            this->colorShader->setUniform("EXPOSURE", std::vector{exposure});
            this->colorShader->prepareForRendering();
        }
        if (this->geometry) {
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
