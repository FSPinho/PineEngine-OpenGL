#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace PineEngine {
    class Transform {
    public:
        Transform();

        Transform& moveTo(const glm::vec3 &to);
        Transform& scaleTo(const double &to);
        Transform& setRotationQuaternion(const glm::quat &rotation_);


        const glm::mat4 &getMatrix();

    private:
        glm::vec3 translation;
        glm::quat rotation;
        glm::vec3 scale;

        bool shouldRecomputeTransform = true;

        mutable glm::mat4 transform;
    };
} // namespace PineEngine
