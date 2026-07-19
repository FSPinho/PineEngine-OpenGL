#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <PineEngine/platform/Input/Input.h>
#include <PineEngine/platform/Platform/Platform.h>
#include <PineEngine/platform/InputManager/InputManager.h>
#include <PineEngine/rendering/Renderer/Renderer.h>
#include <PineEngine/rendering/RendererBackend/RendererBackend.h>
#include <PineEngine/rendering/FrameClearer/FrameClearer.h>
#include <PineEngine/rendering/FrameSwapper/FrameSwapper.h>
#include <PineEngine/rendering/GeometryBuffer/GeometryBuffer.h>
#include <PineEngine/rendering/ShaderSet/ShaderSet.h>
#include <PineEngine/rendering/Timer/Timer.h>
#include <PineEngine/util/Time/Time.h>
#include <PineEngine/application/Application/Application.h>


int main() {
    try {
        PineEngine::Application application;

        // PineEngine::Platform platform;
        // PineEngine::InputManager inputManager(platform);
        //
        // PineEngine::RendererBackend rendererBackend(platform);
        // PineEngine::Renderer renderer(rendererBackend);
        //
        // PineEngine::Timer timer(rendererBackend);
        //
        // renderer.addComponent(timer);
        // renderer.removeComponent(timer);


        //
        // renderer.addComponent<PineEngine::FrameClearer>(rendererContext);
        //
        // auto &shader = renderer.addComponent<PineEngine::ShaderSet>(
        //     rendererContext,
        //     std::string("../shaders/001_vertex.glsl"),
        //     std::string("../shaders/001_fragment.glsl")
        // );
        //
        // renderer.addComponent<PineEngine::GeometryBuffer>(
        //     rendererContext,
        //     std::vector<PineEngine::VertexData>{
        //         {
        //             .name = "vertexInPosition",
        //             .data = std::vector{
        //                 0.5f, 0.5f, 0.0f, // top right
        //                 0.5f, -0.5f, 0.0f, // bottom right
        //                 -0.5f, -0.5f, 0.0f, // bottom left
        //                 -0.5f, 0.5f, 0.0f // top left
        //             },
        //             .dimensionality = 3
        //         },
        //         {
        //             .name = "vertexInColor",
        //             .data = std::vector{
        //                 1.0f, 0.0f, 0.0f, 1.0f, // top right
        //                 0.0f, 1.0f, 0.0f, 1.0f, // bottom right
        //                 0.0f, 0.0f, 1.0f, 1.0f, // bottom left
        //                 1.0f, 1.0f, 1.0f, 1.0f // top left
        //             },
        //             .dimensionality = 4
        //         }
        //     },
        //     std::vector<uint32_t>{
        //         {
        //             0, 1, 3, // first triangle
        //             1, 2, 3 // second triangle
        //         }
        //     }
        // );
        //
        // renderer.addComponent<PineEngine::FrameSwapper>(rendererContext);
        //
        // platform.mainLoop([&platform, &inputManager, &renderer, &timer, &shader ] {
        //     shader.setUniform("TIME", std::vector{timer.getElapsed()});
        //     shader.setUniform(
        //         "TRANSFORM",
        //         glm::rotate(glm::mat4(1.0), glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f))
        //     );
        //
        //     inputManager.process();
        //     renderer.process();
        //
        //     timer.trackFPS();
        //
        //     if (inputManager.isKeyPressed(PineEngine::InputKey::ESCAPE)) {
        //         platform.requestStop();
        //     }
        // });
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
