#include "Application.h"

#include <PineEngine/util/Path/Path.h>


namespace PineEngine {
    Application::Application()
        : rendererBackend(this->platform),
          renderer(this->rendererBackend),
          inputManager(this->platform),
          lightPassFrameBuffer(ResourceManager::load<FrameBuffer>(
              Path::inMemory(),
              rendererBackend
          )) {
        LOG_CONSTRUCTOR("Application");

        this->renderProcessObject.setGeometry<GeometryBuffer>(
            Path::inMemory(),
            GeometryPreset::QUAD,
            this->rendererBackend
        );
        this->renderProcessObject.setColorPassShader<GraphicShader>(
            Path::inMemory(),
            Path::inDisk("shaders/PBR_Quad_ColorPass/vertex.glsl"),
            Path::inDisk("shaders/PBR_Quad_ColorPass/fragment.glsl"),
            rendererBackend
        );
        this->renderProcessObject.setShadowVolumeBuffer<VolumeBuffer>(Path::inMemory(), 32, this->rendererBackend);
        this->renderProcessObject.setShadowComputeShader<ComputeShader>(
            Path::inDisk("shaders/PBR_LightPass/compute.glsl"),
            this->rendererBackend
        );

        this->platform.addResizeListener([this](const uint32_t width, const uint32_t height) {
            this->camera.setAspect(static_cast<float>(width) / static_cast<float>(height));
        });
    }

    Application::~Application() {
        LOG_DESTRUCTOR("Application");
    }

    void Application::mainLoop(const std::function<void(const Tick &)> &onTick) {
        this->platform.mainLoop([this, onTick] {
            const auto tick = this->timer.getNextTick();
            this->inputManager.preProcessMouseEvents(tick);

            const auto [width, height] = this->platform.getSurfaceSize();
            this->lightPassFrameBuffer->resize(width, height);
            this->lightPassFrameBuffer->prepareForRendering();

            for (const auto &directionalLight: this->rootScene.getDirectionalLights()) {
                const auto &target = this->camera.getTarget();
                const auto &translation = glm::normalize(glm::vec3(
                                              directionalLight.direction[0], directionalLight.direction[1],
                                              directionalLight.direction[2]
                                          )) * 50.0f;
                Camera lightCamera(translation, target, -10.0f, 10.0f, -10.0f, 10.0f);
                for (auto &object: this->rootScene.getObjects()) {
                    object.performLightPass(tick, lightCamera);
                }

                this->renderProcessObject.performLightComputing(
                    this->lightPassFrameBuffer->getDepthTextureId(),
                    lightCamera
                );
            }

            // Color pass
            this->rendererBackend.prepareFrameBufferForRendering(0, width, height);
            this->renderer.clearFrame();
            // this->renderProcessObject.performColorPassRendering(
            //     tick,
            //     this->camera,
            //     this->lightPassFrameBuffer->getDepthTextureId()
            // );
            for (auto &object: this->rootScene.getObjects()) {
                object.performColorPass(tick, this->camera);
            }

            this->renderer.commitFrame();

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
} // namespace PineEngine
