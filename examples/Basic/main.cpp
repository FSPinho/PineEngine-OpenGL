#include <iostream>
#include <filesystem>
#include <PineEngine/editor/Editor/Editor.h>
#include <PineEngine/application/Objects/MeshObject/MeshObject.h>

using namespace PineEngine;

int main() {
    try {
        Path::setResourceRootFolder("../examples/Basic/assets");

        Application application;
        auto &rb = application.getRendererBackend();

        auto composed_001 = std::make_unique<MeshObject>();
        composed_001->setGeometry<GeometryBuffer>(Path::RESOURCE("models/_002_rounded_objects.glb"), rb);
        composed_001->setMaterial({.roughness = 1.0f});
        application.getRootScene().addChild(std::move(composed_001));

        application.getRootScene().addChild(std::make_unique<DirectionalLight>(DirectionalLight{
            .direction = {4.0f, 4.0f, 4.0f},
            .irradiance = {1000.0f, 1000.0f, 1000.0f},
            .enableShadows = false, .enableSSAO = true
        }));
        application.getRootScene().addChild(std::make_unique<DirectionalLight>(DirectionalLight{
            .direction = {-1.0f, -1.0f, -1.0f},
            .irradiance = {200.0f, 200.0f, 200.0f},
            .enableSpecular = false,
        }));

        Editor editor(application);
        editor.mainLoop();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
