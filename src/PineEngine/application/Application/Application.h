#pragma once

#include <PineEngine/application/Scene/Scene.h>
#include <PineEngine/application/Camera/Camera.h>
#include <PineEngine/platform/InputManager/InputManager.h>
#include <PineEngine/platform/Platform/Platform.h>
#include <PineEngine/rendering/Renderer/Renderer.h>
#include <PineEngine/rendering/RendererBackend/RendererBackend.h>
#include <PineEngine/util/Path/Path.h>
#include <PineEngine/util/Timer/Timer.h>
#include <PineEngine/util/ResourceHandler/ResourceHandler.h>
#include <PineEngine/util/ResourceManager/ResourceManager.h>

namespace PineEngine {
class Application {
  public:
    explicit Application();
    ~Application();

    void mainLoop();

    RendererBackend &getRendererBackend();
    ResourceHandler<Scene> &getRootScene();

  private:
    Platform platform;
    RendererBackend rendererBackend;
    Renderer renderer;
    InputManager inputManager;
    ResourceHandler<Scene> rootScene = ResourceManager::load<Scene>(Path::inMemory("scenes/_root"));
    Camera camera = Camera(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0, 45.0f);
    Timer timer;
};
} // namespace PineEngine