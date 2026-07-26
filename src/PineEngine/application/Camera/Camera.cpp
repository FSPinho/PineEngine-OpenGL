#include "Camera.h"

namespace PineEngine {
    Camera::Camera(const glm::vec3 &translation, const glm::vec3 &target, const float &aspect, const float &fov)
        : translation(translation), target(target), aspect(aspect), fov(fov), isOrthographic(false) {
    }

    Camera::Camera(
        const glm::vec3 &translation,
        const glm::vec3 &target,
        const float &left,
        const float &right,
        const float &bottom,
        const float &top)
        : translation(translation),
          target(target), left(left), right(right), bottom(bottom),
          top(top), isOrthographic(true) {
    }

    glm::mat4 Camera::getViewMatrix() const {
        if (this->shouldRecomputeViewMatrix) {
            this->viewMatrix = glm::lookAt(this->translation, this->target, glm::vec3(0.0f, 1.0f, 0.0f));
            this->shouldRecomputeViewMatrix = false;
        }
        return this->viewMatrix;
    }

    glm::mat4 Camera::getProjectionMatrix() const {
        if (this->shouldRecomputeProjectionMatrix) {
            if (this->isOrthographic) {
                this->projectionMatrix = glm::ortho(this->left, this->right, this->bottom, this->top, 0.1f, 100.0f);
            } else {
                this->projectionMatrix = glm::perspective(this->fov, this->aspect, 0.1f, 100.0f);
            }
            this->shouldRecomputeProjectionMatrix = false;
        }
        return this->projectionMatrix;
    }

    void Camera::setAspect(const float aspect_) {
        this->aspect = aspect_;
        this->shouldRecomputeProjectionMatrix = true;
    }

    float Camera::getAspect() const {
        return this->aspect;
    }

    const glm::vec3 &Camera::getTarget() const {
        return this->target;
    }

    void Camera::move(const float &forward, const float &sideways) {
        const auto forwardVector = glm::normalize(this->target - this->translation);
        const auto sideVector = glm::normalize(glm::cross(forwardVector, glm::vec3(0.0f, 1.0f, 0.0f)));

        const auto forwardStep = forwardVector * forward;
        const auto sideStep = sideVector * sideways;

        this->translation += forwardStep;
        this->translation += sideStep;
        // this->target += forwardStep;
        // this->target += sideStep;

        this->shouldRecomputeViewMatrix = true;
    }

    void Camera::rotate(const float &horizontal, const float &vertical) {
        const auto forwardVector = this->translation - this->target;
        const auto sideVector = glm::normalize(glm::cross(forwardVector, glm::vec3(0.0f, 1.0f, 0.0f)));

        const auto transform = glm::rotate(glm::rotate(glm::mat4(1.0f), vertical, sideVector), horizontal,
                                           glm::vec3(0.0f, 1.0f, 0.0f));
        const auto forwardVectorTransformed = transform * glm::vec4(forwardVector, 0.0f);
        this->translation = this->target + glm::vec3(forwardVectorTransformed);

        this->shouldRecomputeViewMatrix = true;
    }

    std::vector<float> Camera::getTranslation() const {
        return {this->translation.x, this->translation.y, this->translation.z};
    }
} // namespace PineEngine
