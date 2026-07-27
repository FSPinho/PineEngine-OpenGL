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
        composed_001.setShadowMapShader<GraphicShader>(
            Path::inMemory(),
            Path::inDisk("shaders/PBR_01_ShadowMap/vertex.glsl"),
            Path::inDisk("shaders/PBR_01_ShadowMap/fragment.glsl"),
            rb
        );
        composed_001.setColorShader<GraphicShader>(
            Path::inMemory(),
            Path::inDisk("shaders/PBR_02_Color/vertex.glsl"),
            Path::inDisk("shaders/PBR_02_Color/fragment.glsl"),
            rb
        );
        application.getRootScene().addChild(std::move(composed_001));

        std::vector<DirectionalLight> dLights{
            {
                .direction = {1.0f, 4.0f, 1.0f},
                .irradiance = {1000.0f, 1000.0f, 1000.0f},
                .enableShadows = false, .enableSSAO = true
            },
            {.direction = {0.0f, -4.0f, -1.0f}, .irradiance = {250.0f, 250.0f, 250.0f}},
        };
        std::vector<PointLight> pLights{
            // {.translation = {2.0f, 5.0f, 2.0f}, .radiantIntensity = {5.0f, 5.0f, 5.0f}},
            // {.translation = {-2.0f, 5.0f, -2.0f}, .radiantIntensity = {20.0f, 20.0f, 20.0f}},
        };

        for (auto &light: dLights) {
            application.getRootScene().addChild(std::move(light));
        }
        for (auto &light: pLights) {
            auto lightRef = Object();
            lightRef.setGeometry<GeometryBuffer>(Path::inMemory(), GeometryPreset::SPHERE, rb);
            lightRef.setColorShader<GraphicShader>(
                Path::inMemory(),
                Path::inDisk("shaders/PBR_02_EmitterColor/vertex.glsl"),
                Path::inDisk("shaders/PBR_02_EmitterColor/fragment.glsl"),
                rb
            );
            lightRef.getTransform().moveTo(light.translation);
            lightRef.getTransform().scaleTo(0.2);
            application.getRootScene().addChild(std::move(lightRef));
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
