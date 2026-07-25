#include "Application.h"

#include <PineEngine/util/Path/Path.h>


namespace PineEngine {
    Application::Application()
        : rendererBackend(this->platform),
          renderer(this->rendererBackend),
          inputManager(this->platform),
          positionPassFrameBuffer(ResourceManager::load<FrameBuffer>(
              Path::inMemory("frameBuffers/positionPass"),
              rendererBackend
          )) {
        LOG_CONSTRUCTOR("Application");

        this->colorPassObject.setGeometry<GeometryBuffer>(
            Path::inMemory("geometry/quad/1"),
            GeometryPreset::QUAD,
            this->rendererBackend
        );
        this->colorPassObject.setColorPassShader<GraphicShader>(
            Path::inMemory("shaders/colorPass"),
            Path::inDisk("shaders/colorPass/vertex.glsl"),
            Path::inDisk("shaders/colorPass/fragment.glsl"),
            rendererBackend
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

            // Position Pass
            this->positionPassFrameBuffer->resize(width, height);
            this->positionPassFrameBuffer->prepareForRendering();
            for (auto &object: this->rootScene.getObjects()) {
                object.performPositionPassRendering(tick, this->camera);
            }

            // Color pass
            this->rendererBackend.prepareFrameBufferForRendering(0, width, height);
            this->renderer.clearFrame();
            this->colorPassObject.performColorPassRendering(
                tick,
                this->camera,
                this->positionPassFrameBuffer->getColorTextureId()
            );

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
