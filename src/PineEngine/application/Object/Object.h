#pragma once

#include <PineEngine/application/Camera/Camera.h>
#include <PineEngine/application/Light/Light.h>
#include <PineEngine/rendering/GeometryBuffer/GeometryBuffer.h>
#include <PineEngine/rendering/VolumeBuffer/VolumeBuffer.h>
#include <PineEngine/rendering/Shading/ComputeShader/ComputeShader.h>
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
        void setPositionPassShader(Args... args) {
            this->positionPassShader = ResourceManager::load<T>(args...);
        }

        template<typename T, typename... Args>
        void setColorPassShader(Args... args) {
            this->colorPassShader = ResourceManager::load<T>(args...);
        }

        template<typename T, typename... Args>
        void setGraphicShader(Args... args) {
            this->graphicShader = ResourceManager::load<T>(args...);
        }

        template<typename T, typename... Args>
        void setShadowVolumeBuffer(Args... args) {
            this->shadowVolumeBuffer = ResourceManager::load<T>(args...);
        }

        template<typename T, typename... Args>
        void setShadowComputeShader(Args... args) {
            this->shadowComputeShader = ResourceManager::load<T>(args...);
        }

        void performPositionPassRendering(
            const Tick &tick,
            const Camera &camera
        );

        void performColorPassRendering(
            const Tick &tick,
            const Camera &camera,
            uint32_t positionTextureId
        );

        void performShadowVolumeComputing(uint32_t positionTextureId);

        void performRendering(
            const Tick &tick,
            const Camera &camera,
            const std::vector<PointLight> &pointLights,
            const std::vector<DirectionalLight> &directionalLights
        );

        bool operator==(const Object &other) const;


    private:
        ID id;

        Transform transform;
        ResourceHandler<GeometryBuffer> geometry;
        ResourceHandler<GraphicShader> positionPassShader;
        ResourceHandler<GraphicShader> colorPassShader;
        ResourceHandler<GraphicShader> graphicShader;

        ResourceHandler<VolumeBuffer> shadowVolumeBuffer;
        ResourceHandler<ComputeShader> shadowComputeShader;
    };
} // namespace PineEngine
