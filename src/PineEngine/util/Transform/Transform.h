#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace PineEngine {
class Transform {
  public:
    Transform();

    void setRotationQuaternion(const glm::quat &rotation_);

    const glm::mat4 &getMatrix();

  private:
    glm::vec3 translation;
    glm::quat rotation;
    glm::vec3 scale;

    glm::mat4 transform;
};
} // namespace PineEngine