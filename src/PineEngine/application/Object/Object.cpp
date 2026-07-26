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

    void Object::performLightPass(const Tick &tick, const Camera &camera) {
        if (this->lightPassShader) {
            this->lightPassShader->setUniform("TIME", static_cast<float>(tick.elapsed));
            this->lightPassShader->setUniform("MODEL_MATRIX", this->transform.getMatrix());
            this->lightPassShader->setUniform("VIEW_MATRIX", camera.getViewMatrix());
            this->lightPassShader->setUniform("VIEW_POSITION", camera.getTranslation());
            this->lightPassShader->setUniform("PROJECTION_MATRIX", camera.getProjectionMatrix());
            this->lightPassShader->prepareForRendering();
        }
        if (this->geometry) {
            this->geometry->performRendering();
        }
    }

    void Object::performLightComputing(const uint32_t lightDepthId, const Camera &camera) {
        if (this->lightComputeShader && this->lightVolumeBuffer) {
            constexpr uint32_t volumeBufferAttributeIndex = 0;
            this->lightVolumeBuffer->prepareForCompute(volumeBufferAttributeIndex);
            this->lightComputeShader->setInvocationCount(1, 1);
            this->lightComputeShader->setUniformTexture("DEPTH", lightDepthId);
            this->lightComputeShader->setUniform(
                    "VOLUME_RESOLUTION",
                    std::vector{this->lightVolumeBuffer->getResolution()}
                );
            this->lightComputeShader->setUniform("VIEW_MATRIX", camera.getViewMatrix());
            this->lightComputeShader->setUniform("PROJECTION_MATRIX", camera.getProjectionMatrix());
            this->lightComputeShader->prepareForRendering();
            this->lightComputeShader->executeCompute();
        }
    }

    void Object::performColorPass(const Tick &tick, const Camera &camera) {
        if (this->graphicShader) {
            this->graphicShader->setUniform("TIME", static_cast<float>(tick.elapsed));
            this->graphicShader->setUniform("MODEL_MATRIX", this->transform.getMatrix());
            this->graphicShader->setUniform("VIEW_MATRIX", camera.getViewMatrix());
            this->graphicShader->setUniform("VIEW_POSITION", camera.getTranslation());
            this->graphicShader->setUniform("PROJECTION_MATRIX", camera.getProjectionMatrix());
            this->graphicShader->prepareForRendering();
        }
        if (this->geometry) {
            this->geometry->performRendering();
        }
    }

    void Object::performQuadColorPass(const Tick &tick, const Camera &camera, const uint32_t colorTextureId) {
        if (this->colorPassShader) {
            this->colorPassShader->setUniformTexture("COLOR", colorTextureId);
            this->colorPassShader->setUniform("VIEW_MATRIX", camera.getViewMatrix());
            this->colorPassShader->setUniform("PROJECTION_MATRIX", camera.getProjectionMatrix());

            if (this->lightVolumeBuffer) {
                constexpr uint32_t volumeBufferAttributeIndex = 0;
                this->lightVolumeBuffer->prepareForCompute(volumeBufferAttributeIndex);
                this->colorPassShader->setUniform(
                    "VOLUME_RESOLUTION",
                    std::vector{this->lightVolumeBuffer->getResolution()}
                );
            }
            this->colorPassShader->prepareForRendering();
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
