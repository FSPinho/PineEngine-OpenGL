#include "Camera.h"

namespace PineEngine {
Camera::Camera(const glm::vec3 &translation, const glm::vec3 &target, const float &aspect, const float &fov)
    : translation(translation), target(target), aspect(aspect), fov(fov) {}

glm::mat4 Camera::getViewMatrix() const {
    // TODO: Cache it
    this->viewMatrix = glm::lookAt(this->translation, this->target, glm::vec3(0.0f, 1.0f, 0.0f));
    return this->viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix() const {
    // TODO: Cache it
    this->projectionMatrix = glm::perspective(this->fov, this->aspect, 0.1f, 100.0f);
    return this->projectionMatrix;
}
void Camera::setAspect(const float aspect_) {
    this->aspect = aspect_;
}

float Camera::getAspect() const {
    return this->aspect;
}
} // namespace PineEngine
