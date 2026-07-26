#include <PineEngine/editor/Editor/Editor.h>
#include <PineEngine/util/Time/Time.h>
#include <iostream>

using namespace PineEngine;

int main() {
    try {
        Path::setInDiskRootFolder("../examples/Basic");

        Application application;
        auto &rb = application.getRendererBackend();

        auto composed_001 = Object();
        composed_001.setGeometry<GeometryBuffer>(Path::inDisk("models/composed_002.glb"), rb);
        composed_001.setLightPassShader<GraphicShader>(
            Path::inMemory(),
            Path::inDisk("shaders/PBR_LightPass/vertex.glsl"),
            Path::inDisk("shaders/PBR_LightPass/fragment.glsl"),
            rb
        );
        composed_001.setGraphicShader<GraphicShader>(
            Path::inMemory(),
            Path::inDisk("shaders/PBR_ColorPass/vertex.glsl"),
            Path::inDisk("shaders/PBR_ColorPass/fragment.glsl"),
            rb
        );
        application.getRootScene().addChild(std::move(composed_001));

        std::vector<DirectionalLight> dLights{
            {.direction = {4.0f, 1.0f, 2.0f}, .irradiance = {1000.0f, 1000.0f, 1000.0f}},
        };
        std::vector<PointLight> pLights{
            // {.translation = {4.0f, 4.0f, 4.0f}, .radiantIntensity = {60.0f, 60.0f, 60.0f}},
        };

        for (auto &light: dLights) {
            application.getRootScene().addChild(std::move(light));
        }
        for (auto &light: pLights) {
            application.getRootScene().addChild(std::move(light));
        }

        Editor editor(application);
        editor.mainLoop();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
