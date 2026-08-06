#include "./BaseObject.h"

namespace PineEngine {
    BaseObject::BaseObject() : id(SerialID::generate()) {
        LOG_CONSTRUCTOR(FORMAT("BaseObject[{}]", this->id));
    }

    BaseObject::~BaseObject() {
        LOG_DESTRUCTOR(FORMAT("BaseObject[{}]", this->id));
    }

    Transform &BaseObject::getTransform() {
        return this->transform;
    }

    bool BaseObject::operator==(const BaseObject &other) const {
        return this->id == other.id;
    }
} // namespace PineEngine
