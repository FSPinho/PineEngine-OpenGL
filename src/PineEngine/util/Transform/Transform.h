#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>


namespace PineEngine {
    class Transform {
    public:
        Transform();

        const glm::mat4& getMatrix();

    private:
        glm::vec3 translation;
        glm::quat rotation;
        glm::vec3 scale;

        glm::mat4 transform;
    };
}