#pragma once

#include <PineEngine/application/Camera/Camera.h>
#include <PineEngine/application/Light/Light.h>
#include <PineEngine/rendering/GeometryBuffer/GeometryBuffer.h>
#include <PineEngine/rendering/VolumeBuffer/VolumeBuffer.h>
#include <PineEngine/rendering/Shading/GraphicShader/GraphicShader.h>
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
        GeometryBuffer &getGeometry();

        template<typename T, typename... Args>
        void setGeometry(Args... args) {
            this->geometry = ResourceManager::load<T>(args...);
        }

        template<typename T, typename... Args>
        void setShadowMapShader(Args... args) {
            this->shadowMapShader = ResourceManager::load<T>(args...);
        }

        template<typename T, typename... Args>
        void setColorShader(Args... args) {
            this->colorShader = ResourceManager::load<T>(args...);
        }

        void performShadowMapPass(
            const Tick &tick,
            const Camera &lightCamera
        );

        void performColorPass(
            const Tick &tick,
            const Camera &camera,
            const Camera &lightCamera,
            const DirectionalLight &directionalLight,
            const uint32_t &shadowMapTextureId
        );

        void performAddColorPass(uint32_t colorTextureId, bool multisampled);

        void performPostColorPass(
            const uint32_t &addColorTextureId,
            const std::vector<DirectionalLight> &directionalLights,
            const std::vector<PointLight> &pointLights,
            bool multisampled = false
        );

        bool operator==(const Object &other) const;

    private:
        ID id;

        Transform transform;
        ResourceHandler<GeometryBuffer> geometry;
        ResourceHandler<GraphicShader> shadowMapShader;
        ResourceHandler<GraphicShader> colorShader;
    };
} // namespace PineEngine
