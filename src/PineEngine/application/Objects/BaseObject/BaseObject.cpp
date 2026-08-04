#include "./BaseObject.h"

#include <numbers>

namespace PineEngine {
    BaseObject::BaseObject() : id(SerialID::generate()) {
        LOG_CONSTRUCTOR(FORMAT("BaseObject[{}]", this->id));
    }

    BaseObject::~BaseObject() {
        LOG_DESTRUCTOR(FORMAT("BaseObject[{}]", this->id));
    }

    GeometryBuffer &BaseObject::getGeometry() {
        return *(this->geometry);
    }

    void BaseObject::setMaterial(const Material &material_) {
        this->material = material_;
    }

    void BaseObject::performCubeMapPass(const Tick &tick, const Camera &camera, const uint32_t &cubeMapTextureId) {
        if (this->colorShader) {
            this->transform.moveTo(camera.getTranslation());
            this->colorShader->setUniformCubeMapTexture("CUBE", cubeMapTextureId);
            this->colorShader->setUniform("MODEL_MATRIX", this->transform.getMatrix());
            this->colorShader->setUniform("VIEW_MATRIX", camera.getViewMatrix());
            this->colorShader->setUniform("PROJECTION_MATRIX", camera.getProjectionMatrix());
            this->colorShader->prepareForRendering();
        }
        if (this->geometry) {
            this->geometry->performRendering();
        }
    }

    void BaseObject::performShadowMapPass(const Tick &tick, const Camera &lightCamera) {
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

    void BaseObject::performColorPass(
        const Tick &tick,
        const Camera &camera,
        const Camera &lightCamera,
        const DirectionalLight *directionalLight,
        const PointLight *pointLight,
        const uint32_t &environmentCubeMapTextureId,
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

            this->colorShader->setUniformCubeMapTexture("ENVIRONMENT_CUBE_MAP", environmentCubeMapTextureId);
            this->colorShader->setUniformTexture("SHADOW_MAP", shadowMapTextureId, false, 1);

            this->colorShader->setUniform("ALBEDO", this->material.albedo);
            this->colorShader->setUniform("ROUGHNESS", std::vector{this->material.roughness});
            this->colorShader->setUniform("REFLECTANCE", this->material.reflectance);
            this->colorShader->setUniform("METALLIC", std::vector{this->material.metallic});
            this->colorShader->setUniform("TRANSMISSION", std::vector{this->material.transmission});
            this->colorShader->setUniform("TRANSMISSION_TINT", this->material.transmissionTint);

            this->colorShader->setUniform("DIRECTIONAL_LIGHT.enabled", std::vector{directionalLight == nullptr ? 0u : 1u});
            this->colorShader->setUniform("POINT_LIGHT.enabled", std::vector{pointLight == nullptr ? 0u : 1u});

            if (directionalLight != nullptr) {
                this->colorShader->setUniform("DIRECTIONAL_LIGHT.direction", directionalLight->direction);
                this->colorShader->setUniform("DIRECTIONAL_LIGHT.irradiance", directionalLight->irradiance);
                this->colorShader->setUniform("DIRECTIONAL_LIGHT.enableShadows", std::vector{static_cast<uint32_t>(directionalLight->enableShadows)});
                this->colorShader->setUniform("DIRECTIONAL_LIGHT.enableSSAO", std::vector{static_cast<uint32_t>(directionalLight->enableSSAO)});
                this->colorShader->setUniform("DIRECTIONAL_LIGHT.enableSpecular", std::vector{static_cast<uint32_t>(directionalLight->enableSpecular)});
            } else if (pointLight != nullptr) {
                this->colorShader->setUniform("POINT_LIGHT.translation", pointLight->translation);
                this->colorShader->setUniform("POINT_LIGHT.radiantIntensity", pointLight->radiantIntensity);
                this->colorShader->setUniform("POINT_LIGHT.enableShadows", std::vector{static_cast<uint32_t>(pointLight->enableShadows)});
                this->colorShader->setUniform("POINT_LIGHT.enableSSAO", std::vector{static_cast<uint32_t>(pointLight->enableSSAO)});
                this->colorShader->setUniform("POINT_LIGHT.enableSpecular", std::vector{static_cast<uint32_t>(pointLight->enableSpecular)});
            }

            this->colorShader->prepareForRendering();
        }
        if (this->geometry) {
            this->geometry->performRendering();
        }
    }

    void BaseObject::performAddColorPass(const uint32_t colorTextureId, const bool multisampled) {
        if (this->colorShader) {
            this->colorShader->setUniformTexture("COLOR", colorTextureId, multisampled);
            this->colorShader->prepareForRendering();
        }
        if (this->geometry) {
            this->geometry->performRendering();
        }
    }

    void BaseObject::performPostColorPass(
        const uint32_t &environmentTextureId,
        const uint32_t &addColorTextureId,
        const std::vector<std::unique_ptr<DirectionalLight> > &directionalLights,
        const std::vector<std::unique_ptr<PointLight> > &pointLights
    ) {
        float lightIntensity = 0.0f;
        for (const auto &light: directionalLights) {
            lightIntensity += glm::length(light->irradiance);
        }
        for (const auto &light: pointLights) {
            lightIntensity = glm::length(light->radiantIntensity) / std::pow(glm::length(light->translation), 2.0f);
        }
        const float exposure = 1.0f / (lightIntensity / static_cast<float>(std::numbers::pi));

        if (this->colorShader) {
            this->colorShader->setUniformTexture("ENVIRONMENT", environmentTextureId, false, 0);
            this->colorShader->setUniformTexture("COLOR", addColorTextureId, false, 1);
            this->colorShader->setUniform("EXPOSURE", std::vector{exposure});
            this->colorShader->prepareForRendering();
        }
        if (this->geometry) {
            this->geometry->performRendering();
        }
    }

    void BaseObject::markAsLightRef() {
        this->isLightRef = true;
    }

    bool BaseObject::operator==(const BaseObject &other) const {
        return this->id == other.id;
    }

    Transform &BaseObject::getTransform() {
        return this->transform;
    }
} // namespace PineEngine
