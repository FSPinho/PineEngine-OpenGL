#pragma once

#include <PineEngine/application/Camera/Camera.h>
#include <PineEngine/application/Light/Light.h>
#include <PineEngine/rendering/GeometryBuffer/GeometryBuffer.h>
#include <PineEngine/rendering/ShaderSet/ShaderSet.h>
#include <PineEngine/util/ResourceHandler/ResourceHandler.h>
#include <PineEngine/util/ResourceManager/ResourceManager.h>
#include <PineEngine/util/Transform/Transform.h>
#include <PineEngine/util/Timer/Timer.h>
#include <PineEngine/util/SerialID/SerialID.h>


namespace PineEngine {
    class Object {
    public:
        explicit Object();
        ~Object();

        Transform &getTransform();

        template<typename T, typename... Args>
        void setGeometry(Args... args) {
            this->geometry = ResourceManager::load<T>(args...);
        }

        template<typename T, typename... Args>
        void setShaderSet(Args... args) {
            this->shaderSet = ResourceManager::load<T>(args...);
        }

        GeometryBuffer &getGeometry();

        void performRendering(
            const Tick &tick,
            const Camera &camera,
            const std::vector<PointLight> &pointLights
        );

        bool operator==(const Object &other) const;

    private:
        ID id;

        Transform transform;
        ResourceHandler<GeometryBuffer> geometry;
        ResourceHandler<ShaderSet> shaderSet;
    };
} // namespace PineEngine
