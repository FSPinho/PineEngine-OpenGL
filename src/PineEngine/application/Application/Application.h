#pragma once

#include <functional>
#include <PineEngine/application/Object/Object.h>
#include <PineEngine/application/Scene/Scene.h>
#include <PineEngine/application/Camera/Camera.h>
#include <PineEngine/platform/InputManager/InputManager.h>
#include <PineEngine/platform/Platform/Platform.h>
#include <PineEngine/rendering/Renderer/Renderer.h>
#include <PineEngine/rendering/RendererBackend/RendererBackend.h>
#include <PineEngine/rendering/FrameBuffer/FrameBuffer.h>
#include <PineEngine/util/Timer/Timer.h>

namespace PineEngine {
    class Application {
        friend class Scene;

    public:
        explicit Application();
        ~Application();

        void mainLoop(const std::function<void(const Tick &)> &onTick);

        RendererBackend &getRendererBackend();
        Scene &getRootScene();
        Camera &getCamera();
        InputManager &getInputManager();

    private:
        Platform platform;
        RendererBackend rendererBackend;
        Renderer renderer;
        InputManager inputManager;
        Camera camera = Camera(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0, 45.0f);
        Timer timer;

        Scene rootScene;

        ResourceHandler<FrameBuffer> positionPassFrameBuffer;
        Object colorPassObject;
    };
} // namespace PineEngine
