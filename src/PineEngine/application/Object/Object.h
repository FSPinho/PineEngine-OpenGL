#pragma once

#include <PineEngine/application/Camera/Camera.h>
#include <PineEngine/rendering/GeometryBuffer/GeometryBuffer.h>
#include <PineEngine/rendering/ShaderSet/ShaderSet.h>
#include <PineEngine/util/Path/Path.h>
#include <PineEngine/util/Resource/Resource.h>
#include <PineEngine/util/ResourceHandler/ResourceHandler.h>
#include <PineEngine/util/Transform/Transform.h>
#include <optional>

namespace PineEngine {
class Object : public Resource {
  public:
    explicit Object(const Path &path);
    ~Object() override;

    void setGeometry(ResourceHandler<GeometryBuffer> &&geometry_) { this->geometry = std::move(geometry_); }
    void setShaderSet(ResourceHandler<ShaderSet> &&shaderSet_) { this->shaderSet = std::move(shaderSet_); }

    void performRendering(const double &time, const Camera &camera);

  protected:
    void performLoad() override;
    void performUnload() override;

    Transform &getTransform();

  private:
    Transform transform;
    std::optional<ResourceHandler<GeometryBuffer>> geometry;
    std::optional<ResourceHandler<ShaderSet>> shaderSet;
};
} // namespace PineEngine
