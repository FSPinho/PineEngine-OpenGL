#include "Application.h"

#include <PineEngine/util/Path/Path.h>


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
          )) {
        LOG_CONSTRUCTOR("Application");

        this->platform.addResizeListener([this](const uint32_t width, const uint32_t height) {
            this->camera.setAspect(static_cast<float>(width) / static_cast<float>(height));
        });

        this->environmentObject.setGeometry<GeometryBuffer>(GeometryBuffer::CUBE, this->rendererBackend);
        this->environmentObject.setColorShader<GraphicShader>(
            Path::MEMORY(),
            Path::SYSTEM("assets/shaders/PBR_00_CubeMapUse/vertex.glsl"),
            Path::SYSTEM("assets/shaders/PBR_00_CubeMapUse/fragment.glsl"),
            this->rendererBackend
        );

        this->addColorQuadObject.setGeometry<GeometryBuffer>(GeometryBuffer::QUAD, this->rendererBackend);
        this->addColorQuadObject.setColorShader<GraphicShader>(
            Path::MEMORY(),
            Path::SYSTEM("assets/shaders/PBR_03_AddColor/vertex.glsl"),
            Path::SYSTEM("assets/shaders/PBR_03_AddColor/fragment.glsl"),
            this->rendererBackend
        );

        this->postProcessingQuadObject.setGeometry<GeometryBuffer>(GeometryBuffer::QUAD, this->rendererBackend);
        this->postProcessingQuadObject.setColorShader<GraphicShader>(
            Path::MEMORY(),
            Path::SYSTEM("assets/shaders/PBR_04_PostColor/vertex.glsl"),
            Path::SYSTEM("assets/shaders/PBR_04_PostColor/fragment.glsl"),
            this->rendererBackend
        );
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

        this->_performEnvironmentRender(tick);
        this->_performRenderWithDirectionalLights(tick);
        this->_performRenderWithPointLights(tick);

        // Post color pass
        this->rendererBackend.disableBlend();
        this->rendererBackend.prepareFrameBufferForRendering(0, 1, width, height);
        this->rendererBackend.clearFrame();
        this->postProcessingQuadObject.performPostColorPass(
            this->environmentFrameBuffer->getColorTextureId(),
            this->addColorFrameBuffer->getColorTextureId(),
            this->rootScene.getDirectionalLights(),
            this->rootScene.getPointLights()
        );

        this->rendererBackend.swapBuffers();
    }

    void Application::_performEnvironmentRender(const Tick &tick) {
        this->rendererBackend.disableBlend();
        this->environmentFrameBuffer->prepareForRendering();
        this->environmentFrameBuffer->clear();
        this->environmentObject.performCubeMapPass(tick, this->camera, this->environmentCubeMap->getTextureId(0));
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
                for (const auto &object: this->rootScene.getObjects()) {
                    object->performShadowMapPass(tick, lightCamera);
                }
            }

            // Color
            this->rendererBackend.disableBlend();
            this->colorFrameBuffer->prepareForRendering();
            this->colorFrameBuffer->clear();
            for (const auto &object: this->rootScene.getObjects()) {
                object->performColorPass(
                    tick,
                    this->camera,
                    lightCamera,
                    directionalLight.get(),
                    nullptr,
                    this->environmentCubeMap->getTextureId(1),
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
                for (const auto &object: this->rootScene.getObjects()) {
                    object->performShadowMapPass(tick, lightCamera);
                }
            }

            // Color
            this->rendererBackend.disableBlend();
            this->colorFrameBuffer->prepareForRendering();
            this->colorFrameBuffer->clear();
            for (const auto &object: this->rootScene.getObjects()) {
                object->performColorPass(
                    tick,
                    this->camera,
                    lightCamera,
                    nullptr,
                    pointLight.get(),
                    this->environmentCubeMap->getTextureId(1),
                    this->shadowMapFrameBuffer->getDepthTextureId()
                );
            }

            this->_performRenderAddColor();
        }
    }

    void Application::_performRenderAddColor() {
        this->rendererBackend.enableBlendOneOne();
        this->addColorFrameBuffer->prepareForRendering();
        this->addColorQuadObject.performAddColorPass(this->colorFrameBuffer->getColorTextureId(), true);
    }
} // namespace PineEngine
