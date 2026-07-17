#include <iostream>
#include <PineEngine/platform/Input/Input.h>
#include <PineEngine/platform/Platform/Platform.h>
#include <PineEngine/platform/InputManager/InputManager.h>
#include <PineEngine/rendering/Renderer/Renderer.h>
#include <PineEngine/rendering/RendererBackend/RendererBackend.h>
#include <PineEngine/rendering/FrameClearer/FrameClearer.h>
#include <PineEngine/rendering/FrameSwapper/FrameSwapper.h>
#include <PineEngine/rendering/GeometryBuffer/GeometryBuffer.h>
#include <PineEngine/rendering/ShaderSet/ShaderSet.h>


int main() {
    try {
        PineEngine::Platform platform;
        PineEngine::InputManager inputManager(platform);

        PineEngine::RendererBackend rendererContext(platform);
        PineEngine::Renderer renderer(rendererContext);

        renderer.addComponent<PineEngine::FrameClearer>(rendererContext);

        renderer.addComponent<PineEngine::ShaderSet>(
            rendererContext,
            std::string("../shaders/001_vertex.glsl"),
            std::string("../shaders/001_fragment.glsl")
        ).setUniform("color", std::vector{{1.0f, 0.0f, 0.0f, 1.0f}});

        renderer.addComponent<PineEngine::GeometryBuffer>(
            rendererContext,
            std::vector{
                {
                    0.5f, 0.5f, 0.0f, // top right
                    0.5f, -0.5f, 0.0f, // bottom right
                    -0.5f, -0.5f, 0.0f, // bottom left
                    -0.5f, 0.5f, 0.0f // top left
                }
            },
            std::vector{
                {
                    0, 1, 3, // first triangle
                    1, 2, 3 // second triangle
                }
            }
        );

        renderer.addComponent<PineEngine::FrameSwapper>(rendererContext);

        platform.mainLoop([&platform, &inputManager, &renderer] {
            inputManager.process();
            renderer.process();

            if (inputManager.isKeyPressed(PineEngine::InputKey::ESCAPE)) {
                platform.requestStop();
            }
        });
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
