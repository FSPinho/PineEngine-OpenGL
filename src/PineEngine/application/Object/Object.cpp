#include "Object.h"

namespace PineEngine {

Object::Object(const Path &path) : Resource(path) {
    LOG_CONSTRUCTOR(FORMAT("Object[{}]", this->getPath().asString()));
}

Object::~Object() {
    LOG_DESTRUCTOR(FORMAT("Object[{}]", this->getPath().asString()));
}

void Object::performRendering() {
    if (this->geometry.has_value()) {
        this->geometry.value()->performRendering();
    }
}

void Object::performLoad() {}

void Object::performUnload() {}

Transform &Object::getTransform() {
    return this->transform;
}
} // namespace PineEngine
