#pragma once

#include <PineEngine/util/Transform/Transform.h>
#include <glm/glm.hpp>

namespace PineEngine {
    class Camera {
    public:
        explicit Camera(
            const glm::vec3 &translation,
            const glm::vec3 &target,
            const float &aspect,
            const float &fov,
            const float &near,
            const float &far
        );
        explicit Camera(
            const glm::vec3 &translation,
            const glm::vec3 &target,
            const float &left,
            const float &right,
            const float &bottom,
            const float &top,
            const float &near,
            const float &far
        );

        glm::mat4 getViewMatrix() const;
        glm::mat4 getViewMatrixInverse() const;
        glm::mat4 getProjectionMatrix() const;
        glm::mat4 getProjectionMatrixInverse() const;

        void setAspect(float aspect_);
        float getAspect() const;

        glm::vec3 getTranslation() const;
        std::vector<float> getTranslationAsArray() const;
        std::vector<float> getDirectionAsArray() const;

        const glm::vec3 &getTarget() const;

        void move(const float &forward, const float &sideways);
        void rotate(const float &horizontal, const float &vertical);

    private:
        glm::vec3 translation;
        glm::vec3 target;

        bool isOrthographic;

        float near = 0.0f;
        float far = 0.0f;

        // Perspective
        float aspect = 0.0f;
        float fov = 0.0f;

        // Orthographic
        float left = 0.0f;
        float right = 0.0f;
        float bottom = 0.0f;
        float top = 0.0f;

        mutable bool shouldRecomputeViewMatrix = true;
        mutable bool shouldRecomputeViewMatrixInverse = true;
        mutable bool shouldRecomputeProjectionMatrix = true;
        mutable bool shouldRecomputeProjectionMatrixInverse = true;

        mutable glm::mat4 viewMatrix = glm::mat4(1.0f);
        mutable glm::mat4 viewMatrixInverse = glm::mat4(1.0f);
        mutable glm::mat4 projectionMatrix = glm::mat4(1.0f);
        mutable glm::mat4 projectionMatrixInverse = glm::mat4(1.0f);
    };
} // namespace PineEngine
