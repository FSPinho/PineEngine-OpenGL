#pragma once

#include <PineEngine/application/Camera/Camera.h>
#include <PineEngine/application/Objects/BaseObject/BaseObject.h>
#include <PineEngine/rendering/Light/Light.h>
#include <PineEngine/rendering/GeometryBuffer/GeometryBuffer.h>
#include <PineEngine/rendering/VolumeBuffer/VolumeBuffer.h>
#include <PineEngine/rendering/Shading/GraphicShader/GraphicShader.h>
#include <PineEngine/rendering/Material/Material.h>
#include <PineEngine/util/ResourceHandler/ResourceHandler.h>
#include <PineEngine/util/ResourceManager/ResourceManager.h>
#include <PineEngine/util/Transform/Transform.h>
#include <PineEngine/util/Timer/Timer.h>


namespace PineEngine {
    class MeshObject : public BaseObject {
    public:
        explicit MeshObject() = default;
        ~MeshObject() = default;

        GeometryBuffer &getGeometry();

        void setMaterial(const Material &material_);

        template<typename T, typename... Args>
        void setGeometry(Args &&... args) {
            this->geometry = ResourceManager::load<T>(std::forward<Args>(args)...);
            this->setShadowMapShader<GraphicShader>(
                Path::MEMORY(),
                Path::SYSTEM("assets/shaders/PBR_01_ShadowMap/vertex.glsl"),
                Path::SYSTEM("assets/shaders/PBR_01_ShadowMap/fragment.glsl"),
                this->geometry->getRendererBackend()
            );

            if (this->isLightRef) {
                this->setColorShader<GraphicShader>(
                    Path::MEMORY(),
                    Path::SYSTEM("assets/shaders/PBR_02_EmitterColor/vertex.glsl"),
                    Path::SYSTEM("assets/shaders/PBR_02_EmitterColor/fragment.glsl"),
                    this->geometry->getRendererBackend()
                );
            } else {
                this->setColorShader<GraphicShader>(
                    Path::MEMORY(),
                    Path::SYSTEM("assets/shaders/PBR_02_Color/vertex.glsl"),
                    Path::SYSTEM("assets/shaders/PBR_02_Color/fragment.glsl"),
                    this->geometry->getRendererBackend()
                );
            }
        }

        template<typename T, typename... Args>
        void setShadowMapShader(Args &&... args) {
            this->shadowMapShader = ResourceManager::load<T>(std::forward<Args>(args)...);
        }

        template<typename T, typename... Args>
        void setColorShader(Args &&... args) {
            this->colorShader = ResourceManager::load<T>(std::forward<Args>(args)...);
        }

        void performCubeMapPass(
            const Tick &tick,
            const Camera &camera,
            const uint32_t &cubeMapTextureId
        );

        void performShadowMapPass(
            const Tick &tick,
            const Camera &lightCamera
        );

        void performColorPass(
            const Tick &tick,
            const Camera &camera,
            const Camera &lightCamera,
            const DirectionalLight *directionalLight,
            const PointLight *pointLight,
            const uint32_t &environmentCubeMapTextureId,
            const uint32_t &shadowMapTextureId
        );

        void performAddColorPass(uint32_t colorTextureId, bool multisampled);

        void performPostColorPass(
            const uint32_t &environmentTextureId,
            const uint32_t &addColorTextureId,
            const std::vector<std::unique_ptr<DirectionalLight> > &directionalLights,
            const std::vector<std::unique_ptr<PointLight> > &pointLights
        );

        void markAsLightRef();

    private:
        bool isLightRef = false;

        Material material;
        ResourceHandler<GeometryBuffer> geometry;
        ResourceHandler<GraphicShader> shadowMapShader;
        ResourceHandler<GraphicShader> colorShader;
    };
} // namespace PineEngine
