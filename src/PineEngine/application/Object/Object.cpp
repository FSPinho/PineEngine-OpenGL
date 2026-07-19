#include "Object.h"

namespace PineEngine {

Object::Object(const Path &path) : Resource(path) {
    LOG_CONSTRUCTOR(FORMAT("Object[{}]", this->getPath().asString()));
}

Object::~Object() {
    LOG_DESTRUCTOR(FORMAT("Object[{}]", this->getPath().asString()));
}

void Object::performRendering(const double &time, const Camera &camera) {
    this->transform.setRotationQuaternion(glm::quat(glm::vec3(0.0f, glm::radians(-time * 45.0f), 0.0f)));

    if (this->shaderSet.has_value()) {
        this->shaderSet.value()->setUniform("MODEL_MATRIX", this->transform.getMatrix());
        this->shaderSet.value()->setUniform("VIEW_MATRIX", camera.getViewMatrix());
        this->shaderSet.value()->setUniform("PROJECTION_MATRIX", camera.getProjectionMatrix());
        this->shaderSet.value()->performRendering();
    }
    if (this->geometry.has_value()) {
        this->geometry.value()->performRendering();
    }
}

void Object::performLoad() {}

void Object::performUnload() {}

Transform &Object::getTransform() {
    return this->transform;
}
} // namespace PineEngine
