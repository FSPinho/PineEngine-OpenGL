#include "Camera.h"

namespace PineEngine {
    Camera::Camera(
        const glm::vec3 &translation,
        const glm::vec3 &target,
        const float &aspect,
        const float &fov,
        const float &near,
        const float &far
    )
        : translation(translation), target(target), aspect(aspect), fov(fov), near(near), far(far),
          isOrthographic(false) {
    }

    Camera::Camera(
        const glm::vec3 &translation,
        const glm::vec3 &target,
        const float &left,
        const float &right,
        const float &bottom,
        const float &top,
        const float &near,
        const float &far)
        : translation(translation),
          target(target), left(left), right(right), bottom(bottom),
          top(top), near(near), far(far), isOrthographic(true) {
    }

    glm::mat4 Camera::getViewMatrix() const {
        if (this->shouldRecomputeViewMatrix) {
            this->viewMatrix = glm::lookAt(this->translation, this->target, glm::vec3(0.0f, 1.0f, 0.0f));
            this->shouldRecomputeViewMatrix = false;
        }
        return this->viewMatrix;
    }

    glm::mat4 Camera::getViewMatrixInverse() const {
        if (this->shouldRecomputeViewMatrixInverse) {
            this->viewMatrixInverse = glm::inverse(this->getViewMatrix());
            this->shouldRecomputeViewMatrixInverse = false;
        }
        return this->viewMatrixInverse;
    }

    glm::mat4 Camera::getProjectionMatrix() const {
        if (this->shouldRecomputeProjectionMatrix) {
            if (this->isOrthographic) {
                this->projectionMatrix = glm::ortho(
                    this->left,
                    this->right,
                    this->bottom,
                    this->top,
                    this->near,
                    this->far
                );
            } else {
                this->projectionMatrix = glm::perspective(this->fov, this->aspect, this->near, this->far);
            }
            this->shouldRecomputeProjectionMatrix = false;
        }
        return this->projectionMatrix;
    }

    glm::mat4 Camera::getProjectionMatrixInverse() const {
        if (this->shouldRecomputeProjectionMatrixInverse) {
            this->projectionMatrixInverse = glm::inverse(this->getProjectionMatrix());
            this->shouldRecomputeProjectionMatrixInverse = false;
        }
        return this->projectionMatrixInverse;
    }

    void Camera::setAspect(const float aspect_) {
        this->aspect = aspect_;
        this->shouldRecomputeProjectionMatrix = true;
        this->shouldRecomputeProjectionMatrixInverse = true;
    }

    float Camera::getAspect() const {
        return this->aspect;
    }

    std::vector<float> Camera::getTranslationAsArray() const {
        return {this->translation.x, this->translation.y, this->translation.z};
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
        this->shouldRecomputeViewMatrixInverse = true;
    }

    void Camera::rotate(const float &horizontal, const float &vertical) {
        const auto forwardVector = this->translation - this->target;
        const auto sideVector = glm::normalize(glm::cross(forwardVector, glm::vec3(0.0f, 1.0f, 0.0f)));

        const auto transform = glm::rotate(glm::rotate(glm::mat4(1.0f), vertical, sideVector), horizontal,
                                           glm::vec3(0.0f, 1.0f, 0.0f));
        const auto forwardVectorTransformed = transform * glm::vec4(forwardVector, 0.0f);
        this->translation = this->target + glm::vec3(forwardVectorTransformed);

        this->shouldRecomputeViewMatrix = true;
        this->shouldRecomputeViewMatrixInverse = true;
    }
} // namespace PineEngine
