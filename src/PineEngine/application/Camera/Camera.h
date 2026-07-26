#pragma once

#include <PineEngine/util/Transform/Transform.h>
#include <glm/glm.hpp>

namespace PineEngine {
    class Camera {
    public:
        explicit Camera(const glm::vec3 &translation, const glm::vec3 &target, const float &aspect, const float &fov);
        explicit Camera(
            const glm::vec3 &translation,
            const glm::vec3 &target,
            const float &left,
            const float &right,
            const float &bottom,
            const float &top
        );

        glm::mat4 getViewMatrix() const;
        glm::mat4 getProjectionMatrix() const;

        void setAspect(float aspect_);
        float getAspect() const;

        const glm::vec3 &getTarget() const;

        void move(const float &forward, const float &sideways);
        void rotate(const float &horizontal, const float &vertical);
        std::vector<float> getTranslation() const;

    private:
        glm::vec3 translation;
        glm::vec3 target;

        bool isOrthographic;

        // Perspective
        float aspect = 0.0f;
        float fov = 0.0f;

        // Orthographic
        float left = 0.0f;
        float right = 0.0f;
        float bottom = 0.0f;
        float top = 0.0f;

        mutable bool shouldRecomputeViewMatrix = true;
        mutable bool shouldRecomputeProjectionMatrix = true;

        mutable glm::mat4 viewMatrix = glm::mat4(1.0f);
        mutable glm::mat4 projectionMatrix = glm::mat4(1.0f);
    };
} // namespace PineEngine
