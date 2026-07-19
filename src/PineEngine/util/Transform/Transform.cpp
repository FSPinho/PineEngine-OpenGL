#include "Transform.h"

namespace PineEngine {
Transform::Transform()
    : translation(glm::vec3(0.0f, 0.0f, 0.0f)), rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),
      scale(glm::vec3(1.0f, 1.0f, 1.0f)), transform(glm::mat4(1.0f)) {}

void Transform::setRotationQuaternion(const glm::quat &rotation_) {
    this->rotation = rotation_;
}

const glm::mat4 &Transform::getMatrix() {
    this->transform = glm::translate(glm::mat4(1.0f), this->translation) * glm::mat4_cast(this->rotation) *
                      glm::scale(glm::mat4(1.0f), this->scale);
    return this->transform;
}
} // namespace PineEngine
