#pragma once

#include <functional>
#include <PineEngine/application/Object/Object.h>
#include <PineEngine/application/Scene/Scene.h>
#include <PineEngine/application/Camera/Camera.h>
#include <PineEngine/platform/InputManager/InputManager.h>
#include <PineEngine/platform/Platform/Platform.h>
#include <PineEngine/rendering/RendererBackend/RendererBackend.h>
#include <PineEngine/rendering/FrameBuffer/FrameBuffer.h>
#include <PineEngine/rendering/CubeMap/CubeMap.h>
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
        InputManager inputManager;
        Camera camera = Camera(glm::vec3(4.0f, 4.0f, 4.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0, glm::radians(45.0f), 0.1f, 100.0f);
        Timer timer;

        Scene rootScene;

        ResourceHandler<FrameBuffer> environmentFrameBuffer;
        ResourceHandler<FrameBuffer> shadowMapFrameBuffer;
        ResourceHandler<FrameBuffer> colorFrameBuffer;
        ResourceHandler<FrameBuffer> addColorFrameBuffer;
        ResourceHandler<CubeMap> environmentCubeMap;

        Object environmentObject;
        Object addColorQuadObject;
        Object postProcessingQuadObject;

        void _performRender(const Tick &tick);
        void _performEnvironmentRender(const Tick &tick);
        void _performRenderWithDirectionalLights(const Tick &tick);
        void _performRenderWithPointLights(const Tick &tick);
        void _performRenderAddColor();
    };
} // namespace PineEngine
