#pragma once

#include <PineEngine/rendering/GeometryBuffer/GeometryBuffer.h>
#include <PineEngine/rendering/ShaderSet/ShaderSet.h>
#include <PineEngine/util/Path/Path.h>
#include <PineEngine/util/Resource/Resource.h>
#include <PineEngine/util/Transform/Transform.h>

namespace PineEngine {
class Object : public Resource {
  public:
    explicit Object(const Path &path);

    // Transform &getTransform();
    //
    // ShaderSet *getShader();
    // GeometryBuffer *getGeometry();
    //
    // void setShader(ShaderSet *shader_);
    // void setGeometry(GeometryBuffer *geometry_);

  private:
    // Transform transform;
    // ShaderSet *shader = nullptr;
    // GeometryBuffer *geometry = nullptr;
};
} // namespace PineEngine
