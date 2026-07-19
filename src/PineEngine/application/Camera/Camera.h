#pragma once

#include <PineEngine/util/Transform/Transform.h>
#include <glm/glm.hpp>

namespace PineEngine {
class Camera {
  public:
    explicit Camera(const glm::vec3 &translation, const glm::vec3 &target, const float &aspect, const float &fov);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    void setAspect(float aspect_);
    float getAspect() const;

  private:
    glm::vec3 translation;
    glm::vec3 target;
    float aspect;
    float fov;

    mutable glm::mat4 viewMatrix;
    mutable glm::mat4 projectionMatrix;
};
} // namespace PineEngine
