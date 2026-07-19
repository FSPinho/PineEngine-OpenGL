#include "Transform.h"

namespace PineEngine {
Transform::Transform()
    : translation(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)), rotation(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)),
      scale(glm::vec3(1.0f, 1.0f, 1.0f)), transform(glm::mat4(1.0f)) {}

const glm::mat4 &Transform::getMatrix() {
    this->transform = glm::translate(glm::mat4(1.0f), this->translation) * glm::mat4_cast(this->rotation) *
                      glm::scale(glm::mat4(1.0f), this->scale);
    return this->transform;
}
} // namespace PineEngine
