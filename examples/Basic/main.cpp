#include <iostream>
#include <filesystem>
#include <PineEngine/editor/Editor/Editor.h>

using namespace PineEngine;

int main() {
    try {
        Path::setResourceRootFolder("../examples/Basic/assets");

        Application application;
        auto &rb = application.getRendererBackend();

        auto composed_001 = Object();
        composed_001.setGeometry<GeometryBuffer>(Path::RESOURCE("models/_002_rounded_objects.glb"), rb);
        composed_001.setMaterial({.roughness = 1.0f});
        application.getRootScene().addChild(std::move(composed_001));

        std::vector<DirectionalLight> dLights{
            {
                .direction = {4.0f, 4.0f, 4.0f},
                .irradiance = {1000.0f, 1000.0f, 1000.0f},
                .enableShadows = false, .enableSSAO = true
            },
            {
                .direction = {-1.0f, -1.0f, -1.0f},
                .irradiance = {200.0f, 200.0f, 200.0f},
                .enableSpecular = false,
            }
        };
        std::vector<PointLight> pLights{
            // {.translation = {2.0f, 1.0f, 2.0f}, .radiantIntensity = {5.0f, 5.0f, 5.0f}, .enableSSAO = true},
            // {.translation = {-2.0f, 1.0f, -2.0f}, .radiantIntensity = {20.0f, 20.0f, 20.0f}, .enableSSAO = true},
        };

        for (auto &light: dLights) {
            application.getRootScene().addChild(std::move(light));
        }
        for (auto &light: pLights) {
            auto lightRef = Object();
            lightRef.markAsLightRef();
            lightRef.setGeometry<GeometryBuffer>(GeometryBuffer::SPHERE, rb);
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
