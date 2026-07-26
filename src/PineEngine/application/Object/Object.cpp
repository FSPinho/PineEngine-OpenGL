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

    void Object::performPositionPassRendering(const Tick &tick, const Camera &camera) {
        if (this->positionPassShader) {
            this->positionPassShader->setUniform("TIME", static_cast<float>(tick.elapsed));
            this->positionPassShader->setUniform("MODEL_MATRIX", this->transform.getMatrix());
            this->positionPassShader->setUniform("VIEW_MATRIX", camera.getViewMatrix());
            this->positionPassShader->setUniform("VIEW_POSITION", camera.getTranslation());
            this->positionPassShader->setUniform("PROJECTION_MATRIX", camera.getProjectionMatrix());
            this->positionPassShader->prepareForRendering();
        }
        if (this->geometry) {
            this->geometry->performRendering();
        }
    }

    void Object::performColorPassRendering(const Tick &tick, const Camera &camera, const uint32_t positionTextureId) {
        if (this->colorPassShader) {
            this->colorPassShader->setUniformTexture("POSITION", positionTextureId);
            this->colorPassShader->setUniform("VIEW_MATRIX", camera.getViewMatrix());
            this->colorPassShader->setUniform("PROJECTION_MATRIX", camera.getProjectionMatrix());

            if (this->shadowVolumeBuffer) {
                constexpr uint32_t volumeBufferAttributeIndex = 0;
                this->shadowVolumeBuffer->prepareForCompute(volumeBufferAttributeIndex);
                this->colorPassShader->setUniform(
                    "VOLUME_RESOLUTION",
                    std::vector{this->shadowVolumeBuffer->getResolution()}
                );
            }
            this->colorPassShader->prepareForRendering();
        }
        if (this->geometry) {
            this->geometry->performRendering();
        }
    }

    void Object::performShadowVolumeComputing(const uint32_t positionTextureId) {
        if (this->shadowComputeShader && this->shadowVolumeBuffer) {
            constexpr uint32_t volumeBufferAttributeIndex = 0;
            this->shadowVolumeBuffer->prepareForCompute(volumeBufferAttributeIndex);
            this->shadowComputeShader->setInvocationCount(1, 1);
            this->shadowComputeShader->setUniformTexture("POSITION", positionTextureId);
            this->shadowComputeShader->setUniform(
                    "VOLUME_RESOLUTION",
                    std::vector{this->shadowVolumeBuffer->getResolution()}
                );
            this->shadowComputeShader->prepareForRendering();
            this->shadowComputeShader->executeCompute();
        }
    }

    void Object::performRendering(
        const Tick &tick,
        const Camera &camera,
        const std::vector<PointLight> &pointLights,
        const std::vector<DirectionalLight> &directionalLights
    ) {
        if (this->geometry) {
            // if (this->computeShader) {
            // this->computeShader->setUniform(
            //     "POINT_LIGHTS_COUNT",
            //     std::vector{static_cast<uint32_t>(pointLights.size())}
            // );
            // this->computeShader->setUniform(
            //     "DIRECTIONAL_LIGHTS_COUNT",
            //     std::vector{static_cast<uint32_t>(directionalLights.size())}
            // );
            //
            // for (uint32_t i = 0; i < pointLights.size(); i++) {
            //     std::string prefix = std::string("POINT_LIGHTS[") + std::to_string(i) + "].";
            //     this->computeShader->setUniform(prefix + "translation", pointLights[i].translation);
            //     this->computeShader->setUniform(prefix + "radiantIntensity", pointLights[i].radiantIntensity);
            // }
            // for (uint32_t i = 0; i < directionalLights.size(); i++) {
            //     std::string prefix = std::string("DIRECTIONAL_LIGHTS[") + std::to_string(i) + "].";
            //     this->computeShader->setUniform(prefix + "direction", directionalLights[i].direction);
            //     this->computeShader->setUniform(prefix + "irradiance", directionalLights[i].irradiance);
            // }
            // this->geometry->prepareForCompute();
            // this->computeShader->setInvocationCount(
            //     this->geometry->getVertexCount(),
            //     this->geometry->getIndexCount()
            // );
            // this->computeShader->prepareForRendering();
            // }
        }
        if (this->graphicShader) {
            this->graphicShader->setUniform("TIME", static_cast<float>(tick.elapsed));
            this->graphicShader->setUniform("MODEL_MATRIX", this->transform.getMatrix());
            this->graphicShader->setUniform("VIEW_MATRIX", camera.getViewMatrix());
            this->graphicShader->setUniform("VIEW_POSITION", camera.getTranslation());
            this->graphicShader->setUniform("PROJECTION_MATRIX", camera.getProjectionMatrix());

            this->graphicShader->setUniform(
                "POINT_LIGHTS_COUNT",
                std::vector{static_cast<uint32_t>(pointLights.size())}
            );
            this->graphicShader->setUniform(
                "DIRECTIONAL_LIGHTS_COUNT",
                std::vector{static_cast<uint32_t>(directionalLights.size())}
            );

            for (uint32_t i = 0; i < pointLights.size(); i++) {
                std::string prefix = std::string("POINT_LIGHTS[") + std::to_string(i) + "].";
                this->graphicShader->setUniform(prefix + "translation", pointLights[i].translation);
                this->graphicShader->setUniform(prefix + "radiantIntensity", pointLights[i].radiantIntensity);
            }
            for (uint32_t i = 0; i < directionalLights.size(); i++) {
                std::string prefix = std::string("DIRECTIONAL_LIGHTS[") + std::to_string(i) + "].";
                this->graphicShader->setUniform(prefix + "direction", directionalLights[i].direction);
                this->graphicShader->setUniform(prefix + "irradiance", directionalLights[i].irradiance);
            }

            this->graphicShader->prepareForRendering();
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
