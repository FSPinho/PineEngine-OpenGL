#include <PineEngine/editor/Editor/Editor.h>
#include <PineEngine/util/ResourceManager/ResourceManager.h>
#include <PineEngine/util/Time/Time.h>
#include <PineEngine/util/Log/Log.h>
#include <iostream>

using namespace PineEngine;

int main() {
    try {
        Path::setInDiskRootFolder("../examples/Basic");

        Application application;
        auto &rb = application.getRendererBackend();

        auto composed_001 = Object();
        composed_001.setGeometry<GeometryBuffer>(Path::inDisk("models/composed_002.glb"), rb);
        composed_001.setShaderSet<ShaderSet>(Path::inDisk("shaders/pbm"), rb);
        composed_001.getGeometry().enableWireframe();
        application.getRootScene().addChild(std::move(composed_001));

        std::vector<PointLight> lights;
        for (int i = 0; i < 16; i++) {
            float x = -2.0f + 4.0f * static_cast<float>(i) / 16.0f;
            float r = 10.0f * static_cast<float>(i) / 16.0f;
            float b = 10.0f - r;
            lights.emplace_back(PointLight{
                .translation = {x, 2.0f, 4.0f}, .radiantIntensity = {r, 0.0f, b}
            });
        }

        for (auto light: lights) {
            auto lightRef = Object();
            lightRef.getTransform()
                    .moveTo(glm::vec3(light.translation[0], light.translation[1], light.translation[2]))
                    .scaleTo(0.2);;
            lightRef.setGeometry<GeometryBuffer>(Path::inMemory("geo/1"), GeometryPreset::SPHERE, rb);
            lightRef.setShaderSet<ShaderSet>(Path::inDisk("shaders/light"), rb);
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
