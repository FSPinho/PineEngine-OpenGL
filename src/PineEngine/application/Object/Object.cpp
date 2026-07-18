#include "Object.h"


namespace PineEngine {
    Transform &Object::getTransform() {
        return this->transform;
    }

    ShaderSet *Object::getShader() {
        return this->shader;
    }

    GeometryBuffer *Object::getGeometry() {
        return this->geometry;
    }

    void Object::setShader(ShaderSet *shader_) {
        this->shader = shader_;
    }

    void Object::setGeometry(GeometryBuffer *geometry_) {
        this->geometry = geometry_;
    }
}
