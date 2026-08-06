#include "Transform.h"

namespace PineEngine {
    Transform::Transform()
        : translation(glm::vec3(0.0f, 0.0f, 0.0f)), rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),
          scale(glm::vec3(1.0f, 1.0f, 1.0f)), transform(glm::mat4(1.0f)) {
    }

    Transform &Transform::moveTo(const glm::vec3 &to) {
        this->translation = to;
        this->shouldRecomputeTransform = true;
        return *this;
    }

    Transform &Transform::scaleTo(const double &to) {
        this->scale = glm::vec3(to);
        return *this;
    }

    Transform &Transform::setRotationQuaternion(const glm::quat &rotation_) {
        this->rotation = rotation_;
        this->shouldRecomputeTransform = true;
        return *this;
    }

    const glm::mat4 &Transform::getMatrix() {
        if (this->shouldRecomputeTransform) {
            this->transform = glm::translate(glm::mat4(1.0f), this->translation) *
                              glm::mat4_cast(this->rotation) *
                              glm::scale(glm::mat4(1.0f), this->scale);
            this->shouldRecomputeTransform = false;
        }
        return this->transform;
    }
} // namespace PineEngine
