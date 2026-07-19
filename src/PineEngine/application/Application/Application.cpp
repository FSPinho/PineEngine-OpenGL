#include "Application.h"

#include <PineEngine/platform/Input/Input.h>

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

void Application::mainLoop() {
    this->platform.mainLoop([this] {
        this->renderer.startFrame();
        this->rootScene->performRendering(this->timer.getElapsed(), this->camera);
        this->renderer.commitFrame();

        if (this->inputManager.isKeyPressed(InputKey::ESCAPE)) {
            this->platform.requestStop();
        }
    });
}
RendererBackend &Application::getRendererBackend() {
    return this->rendererBackend;
}

ResourceHandler<Scene> &Application::getRootScene() {
    return this->rootScene;
}
} // namespace PineEngine
