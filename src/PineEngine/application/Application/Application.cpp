#include "Application.h"

#include <numbers>
#include <PineEngine/util/Path/Path.h>
#include <PineEngine/util/ResourceManager/ResourceManager.h>
#include <PineEngine/rendering/GeometryBuffer/GeometryBuffer.h>


namespace PineEngine {
    Application::Application()
        : rendererBackend(this->platform),
          inputManager(this->platform),
          environmentFrameBuffer(ResourceManager::load<FrameBuffer>(
              Path::MEMORY(),
              rendererBackend,
              FrameBufferOptions{}
          )),
          shadowMapFrameBuffer(ResourceManager::load<FrameBuffer>(
              Path::MEMORY(),
              rendererBackend,
              FrameBufferOptions{.depth = true}
          )),
          colorFrameBuffer(ResourceManager::load<FrameBuffer>(
              Path::MEMORY(),
              rendererBackend,
              FrameBufferOptions{.multisampled = true, .depth = true}
          )),
          addColorFrameBuffer(ResourceManager::load<FrameBuffer>(
              Path::MEMORY(),
              rendererBackend,
              FrameBufferOptions{}
          )),
          environmentCubeMap(ResourceManager::load<CubeMap>(
              Path::MEMORY(),
              Path::SYSTEM("assets/hdri/woods-001.exr"),
              rendererBackend
          )),
          shadowMapShader(ResourceManager::load<GraphicShader>(
              Path::MEMORY(),
              Path::SYSTEM("assets/shaders/PBR_01_ShadowMap/vertex.glsl"),
              Path::SYSTEM("assets/shaders/PBR_01_ShadowMap/fragment.glsl"),
              this->rendererBackend
          )) {
        LOG_CONSTRUCTOR("Application");

        this->platform.addResizeListener([this](const uint32_t width, const uint32_t height) {
            this->camera.setAspect(static_cast<float>(width) / static_cast<float>(height));
        });

        this->environmentObject.setGeometry(ResourceManager::load<GeometryBuffer>(GeometryBuffer::CUBE, this->rendererBackend));
        this->environmentObject.setGraphicShader(ResourceManager::load<GraphicShader>(
            Path::MEMORY(),
            Path::SYSTEM("assets/shaders/PBR_00_CubeMapUse/vertex.glsl"),
            Path::SYSTEM("assets/shaders/PBR_00_CubeMapUse/fragment.glsl"),
            this->rendererBackend
        ));

        this->addColorQuadObject.setGeometry(ResourceManager::load<GeometryBuffer>(GeometryBuffer::QUAD, this->rendererBackend));
        this->addColorQuadObject.setGraphicShader(ResourceManager::load<GraphicShader>(
            Path::MEMORY(),
            Path::SYSTEM("assets/shaders/PBR_03_AddColor/vertex.glsl"),
            Path::SYSTEM("assets/shaders/PBR_03_AddColor/fragment.glsl"),
            this->rendererBackend
        ));

        this->postProcessingQuadObject.setGeometry(ResourceManager::load<GeometryBuffer>(GeometryBuffer::QUAD, this->rendererBackend));
        this->postProcessingQuadObject.setGraphicShader(ResourceManager::load<GraphicShader>(
            Path::MEMORY(),
            Path::SYSTEM("assets/shaders/PBR_04_PostColor/vertex.glsl"),
            Path::SYSTEM("assets/shaders/PBR_04_PostColor/fragment.glsl"),
            this->rendererBackend
        ));
    }

    Application::~Application() {
        LOG_DESTRUCTOR("Application");
    }

    void Application::mainLoop(const std::function<void(const Tick &)> &onTick) {
        this->platform.mainLoop([this, onTick] {
            const auto tick = this->timer.getNextTick();
            this->inputManager.preProcessMouseEvents(tick);

            this->_performRender(tick);

            onTick(tick);

            this->inputManager.postProcessMouseEvents();
        });
    }

    RendererBackend &Application::getRendererBackend() {
        return this->rendererBackend;
    }

    Scene &Application::getRootScene() {
        return this->rootScene;
    }

    Camera &Application::getCamera() {
        return this->camera;
    }

    InputManager &Application::getInputManager() {
        return this->inputManager;
    }

    void Application::_performRender(const Tick &tick) {
        const auto [width, height] = this->platform.getSurfaceSize();

        this->environmentFrameBuffer->resize(width, height);
        this->shadowMapFrameBuffer->resize(4096, 4096);
        this->colorFrameBuffer->resize(width, height);
        this->addColorFrameBuffer->resize(width, height);

        this->environmentFrameBuffer->attachTextures();
        this->shadowMapFrameBuffer->attachTextures();
        this->colorFrameBuffer->attachTextures();
        this->addColorFrameBuffer->attachTextures();

        this->addColorFrameBuffer->clear();

        this->_performEnvironmentRender();
        this->_performRenderWithDirectionalLights(tick);
        this->_performRenderWithPointLights(tick);
        this->_performRenderPostColor(width, height);

        this->rendererBackend.swapBuffers();
    }

    void Application::_performEnvironmentRender() {
        this->rendererBackend.disableBlend();
        this->environmentFrameBuffer->prepareForRendering();
        this->environmentFrameBuffer->clear();

        auto &transform = this->environmentObject.getTransform();
        auto &shader = this->environmentObject.getGraphicShader();
        auto &geometry = this->environmentObject.getGeometry();
        if (!shader || !geometry) return;

        transform.moveTo(this->camera.getTranslation());
        shader->setUniformCubeMapTexture("CUBE", this->environmentCubeMap->getTextureId(0));
        shader->setUniform("MODEL_MATRIX", transform.getMatrix());
        shader->setUniform("VIEW_MATRIX", this->camera.getViewMatrix());
        shader->setUniform("PROJECTION_MATRIX", this->camera.getProjectionMatrix());
        shader->prepareForRendering();
        geometry->performRendering();
    }

    void Application::_performRenderWithDirectionalLights(const Tick &tick) {
        for (const auto &directionalLight: this->rootScene.getDirectionalLights()) {
            // Shadow map
            this->shadowMapFrameBuffer->prepareForRendering();
            this->shadowMapFrameBuffer->clear();
            const auto &target = this->camera.getTarget();
            const auto &translation = target + glm::normalize(directionalLight->direction) * 50.0f;
            Camera lightCamera(translation, target, -10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
            if (directionalLight->enableShadows || directionalLight->enableSSAO) {
                for (auto &object: this->rootScene.getObjects()) {
                    this->performObjectShadowMapPass(object, lightCamera);
                }
            }

            // Color
            this->rendererBackend.disableBlend();
            this->colorFrameBuffer->prepareForRendering();
            this->colorFrameBuffer->clear();
            for (auto &object: this->rootScene.getObjects()) {
                this->performObjectColorPass(
                    object,
                    tick,
                    lightCamera,
                    directionalLight,
                    std::unique_ptr<PointLight>(nullptr),
                    this->shadowMapFrameBuffer->getDepthTextureId()
                );
            }

            this->_performRenderAddColor();
        }
    }

    void Application::_performRenderWithPointLights(const Tick &tick) {
        for (const auto &pointLight: this->rootScene.getPointLights()) {
            // Shadow map
            this->shadowMapFrameBuffer->prepareForRendering();
            this->shadowMapFrameBuffer->clear();
            const auto &target = this->camera.getTarget();
            const auto &translation = pointLight->translation;
            Camera lightCamera(translation, target, 1.0f, glm::radians(90.0f), 0.1f, 100.0f);
            if (pointLight->enableShadows || pointLight->enableSSAO) {
                for (auto &object: this->rootScene.getObjects()) {
                    this->performObjectShadowMapPass(object, lightCamera);
                }
            }

            // Color
            this->rendererBackend.disableBlend();
            this->colorFrameBuffer->prepareForRendering();
            this->colorFrameBuffer->clear();
            for (auto &object: this->rootScene.getObjects()) {
                performObjectColorPass(
                    object,
                    tick,
                    lightCamera,
                    std::unique_ptr<DirectionalLight>(nullptr),
                    pointLight,
                    this->shadowMapFrameBuffer->getDepthTextureId()
                );
            }

            this->_performRenderAddColor();
        }
    }

    void Application::_performRenderAddColor() {
        auto &geometry = this->addColorQuadObject.getGeometry();
        auto &shader = this->addColorQuadObject.getGraphicShader();
        if (!geometry || !shader) return;

        this->rendererBackend.enableBlendOneOne();
        this->addColorFrameBuffer->prepareForRendering();

        shader->setUniformTexture("COLOR", this->colorFrameBuffer->getColorTextureId(), true);
        shader->prepareForRendering();
        geometry->performRendering();
    }


    void Application::performObjectShadowMapPass(const std::unique_ptr<Object> &object, const Camera &lightCamera) {
        auto &transform = object->getTransform();
        auto &geometry = object->getGeometry();
        if (!geometry) return;

        this->shadowMapShader->setUniform("MODEL_MATRIX", transform.getMatrix());
        this->shadowMapShader->setUniform("VIEW_MATRIX", lightCamera.getViewMatrix());
        this->shadowMapShader->setUniform("PROJECTION_MATRIX", lightCamera.getProjectionMatrix());
        this->shadowMapShader->prepareForRendering();
        geometry->performRendering();
    }

    void Application::performObjectColorPass(
        const std::unique_ptr<Object> &object,
        const Tick &tick,
        const Camera &lightCamera,
        const std::unique_ptr<DirectionalLight> &directionalLight,
        const std::unique_ptr<PointLight> &pointLight,
        const uint32_t &shadowMapTextureId
    ) {
        auto &transform = object->getTransform();
        auto &geometry = object->getGeometry();
        auto &shader = object->getGraphicShader();
        auto &material = object->getMaterial();
        if (!geometry || !shader || !material) return;

        shader->setUniform("TIME", std::vector{static_cast<float>(tick.elapsed)});
        shader->setUniform("MODEL_MATRIX", transform.getMatrix());
        shader->setUniform("VIEW_MATRIX", this->camera.getViewMatrix());
        shader->setUniform("VIEW_MATRIX_INVERSE", this->camera.getViewMatrixInverse());
        shader->setUniform("VIEW_POSITION", this->camera.getTranslationAsArray());
        shader->setUniform("PROJECTION_MATRIX", this->camera.getProjectionMatrix());
        shader->setUniform("PROJECTION_MATRIX_INVERSE", this->camera.getProjectionMatrixInverse());
        shader->setUniform("LIGHT_VIEW_MATRIX", lightCamera.getViewMatrix());
        shader->setUniform("LIGHT_PROJECTION_MATRIX", lightCamera.getProjectionMatrix());

        shader->setUniformCubeMapTexture("ENVIRONMENT_CUBE_MAP", this->environmentCubeMap->getTextureId(1));
        shader->setUniformTexture("SHADOW_MAP", shadowMapTextureId, false, 1);

        shader->setUniform("ALBEDO", material->albedo);
        shader->setUniform("ROUGHNESS", std::vector{material->roughness});
        shader->setUniform("REFLECTANCE", material->reflectance);
        shader->setUniform("METALLIC", std::vector{material->metallic});
        shader->setUniform("TRANSMISSION", std::vector{material->transmission});
        shader->setUniform("TRANSMISSION_TINT", material->transmissionTint);

        shader->setUniform("DIRECTIONAL_LIGHT.enabled", std::vector{!directionalLight ? 0u : 1u});
        shader->setUniform("POINT_LIGHT.enabled", std::vector{!pointLight ? 0u : 1u});

        if (directionalLight) {
            shader->setUniform("DIRECTIONAL_LIGHT.direction", directionalLight->direction);
            shader->setUniform("DIRECTIONAL_LIGHT.irradiance", directionalLight->irradiance);
            shader->setUniform("DIRECTIONAL_LIGHT.enableShadows", std::vector{static_cast<uint32_t>(directionalLight->enableShadows)});
            shader->setUniform("DIRECTIONAL_LIGHT.enableSSAO", std::vector{static_cast<uint32_t>(directionalLight->enableSSAO)});
            shader->setUniform("DIRECTIONAL_LIGHT.enableSpecular", std::vector{static_cast<uint32_t>(directionalLight->enableSpecular)});
        } else if (pointLight) {
            shader->setUniform("POINT_LIGHT.translation", pointLight->translation);
            shader->setUniform("POINT_LIGHT.radiantIntensity", pointLight->radiantIntensity);
            shader->setUniform("POINT_LIGHT.enableShadows", std::vector{static_cast<uint32_t>(pointLight->enableShadows)});
            shader->setUniform("POINT_LIGHT.enableSSAO", std::vector{static_cast<uint32_t>(pointLight->enableSSAO)});
            shader->setUniform("POINT_LIGHT.enableSpecular", std::vector{static_cast<uint32_t>(pointLight->enableSpecular)});
        }

        shader->prepareForRendering();
        geometry->performRendering();
    }

    void Application::_performRenderPostColor(const uint32_t &width, const uint32_t &height) {
        this->rendererBackend.disableBlend();
        this->rendererBackend.prepareFrameBufferForRendering(0, 1, width, height);
        this->rendererBackend.clearFrame();

        auto &geometry = this->postProcessingQuadObject.getGeometry();
        auto &shader = this->postProcessingQuadObject.getGraphicShader();
        if (!geometry || !shader) return;

        float lightIntensity = 0.0f;
        for (const auto &light: this->rootScene.getDirectionalLights()) {
            lightIntensity += glm::length(light->irradiance);
        }
        for (const auto &light: this->rootScene.getPointLights()) {
            lightIntensity = glm::length(light->radiantIntensity) / std::pow(glm::length(light->translation), 2.0f);
        }
        const float exposure = 1.0f / (lightIntensity / static_cast<float>(std::numbers::pi));

        shader->setUniformTexture("ENVIRONMENT", this->environmentFrameBuffer->getColorTextureId(), false, 0);
        shader->setUniformTexture("COLOR", this->addColorFrameBuffer->getColorTextureId(), false, 1);
        shader->setUniform("EXPOSURE", std::vector{exposure});
        shader->prepareForRendering();
        geometry->performRendering();
    }
} // namespace PineEngine
