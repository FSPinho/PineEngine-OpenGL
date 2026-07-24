#include "Application.h"


namespace PineEngine {
    Application::Application()
        : rendererBackend(this->platform), renderer(this->rendererBackend), inputManager(this->platform) {
        LOG_CONSTRUCTOR("Application");

        this->platform.addResizeListener([this](const uint32_t width, const uint32_t height) {
            this->camera.setAspect(static_cast<float>(width) / static_cast<float>(height));
        });
    }

    Application::~Application() {
        LOG_DESTRUCTOR("Application");
    }

    void Application::mainLoop(const std::function<void(const Tick &)> &onTick) {
        this->platform.mainLoop([this, onTick] {
            auto const tick = this->timer.getNextTick();

            this->inputManager.preProcessMouseEvents(tick);

            this->renderer.startFrame();

            for (auto &object: this->getRootScene().getObjects()) {
                object.performRendering(tick, this->camera, this->getRootScene().getPointLights());
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
