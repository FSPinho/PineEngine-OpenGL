#pragma once

#include <PineEngine/util/Transform/Transform.h>
#include <PineEngine/rendering/ShaderSet/ShaderSet.h>
#include <PineEngine/rendering/GeometryBuffer/GeometryBuffer.h>


namespace PineEngine {
    class Object {
    public:
        Transform &getTransform();

        ShaderSet *getShader();
        GeometryBuffer *getGeometry();

        void setShader(ShaderSet *shader_);
        void setGeometry(GeometryBuffer *geometry_);

    private:
        Transform transform;
        ShaderSet *shader = nullptr;
        GeometryBuffer *geometry = nullptr;
    };
}
