#include <iostream>
#include <filesystem>
#include <PineEngine/editor/Editor/Editor.h>
#include <PineEngine/util/ResourceManager/ResourceManager.h>
#include <PineEngine/application/Object/Object.h>

using namespace PineEngine;

int main() {
    try {
        Path::setResourceRootFolder("../examples/Basic/assets");

        Application application;
        auto &rb = application.getRendererBackend();

        auto composed_001 = std::make_unique<Object>();
        // composed_001->setGeometry(ResourceManager::load<GeometryBuffer>(Path::RESOURCE("models/_002_rounded_objects.glb"), rb));
        composed_001->setGeometry(ResourceManager::load<GeometryBuffer>(Path::RESOURCE("models/_003_leaves.glb"), rb));
        composed_001->setGraphicShader(ResourceManager::load<GraphicShader>(
            Path::MEMORY(),
            Path::SYSTEM("assets/shaders/PBR_02_Color/vertex.glsl"),
            Path::SYSTEM("assets/shaders/PBR_02_Color/fragment.glsl"),
            rb
        ));
        // composed_001->setMaterial({.roughness = 1.0f});
        composed_001->setMaterial({.albedo = {0.0f, 1.0f, 0.0f}, .roughness = 1.0, .transmission = 0.5f, .transmissionTint = {0.25f, 1.0f, 0.0f}});
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
