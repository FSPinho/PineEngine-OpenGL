#pragma once

#include <PineEngine/util/Transform/Transform.h>
#include <PineEngine/util/SerialID/SerialID.h>


namespace PineEngine {
    class BaseObject {
    public:
        explicit BaseObject();
        ~BaseObject();

        Transform &getTransform();

        bool operator==(const BaseObject &other) const;

    private:
        ID id;
        Transform transform;
    };
} // namespace PineEngine
