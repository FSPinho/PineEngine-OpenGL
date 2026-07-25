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
        // composed_001.getGeometry().enableWireframe();
        composed_001.setShaderSet<ShaderSet>(Path::inDisk("shaders/pbm"), rb);
        application.getRootScene().addChild(std::move(composed_001));

        std::vector<PointLight> lights;
        uint32_t lightCount = 8;
        for (uint32_t i = 0; i < lightCount; i++) {
            float progress = static_cast<float>(i) / static_cast<float>(lightCount - 1);
            float x = 4.0f * (progress * 2.0f - 1.0f);
            LOG(FORMAT("X={}", x));
            float r = 10.0f * static_cast<float>(i) / static_cast<float>(lightCount);
            float b = 10.0f - r;
            lights.emplace_back(PointLight{
                .translation = {x, 4.0f, 2.0f}, .radiantIntensity = {0.0f + r, 10.0f, 0.0f + b}
            });
        }

        for (auto light: lights) {
            auto lightRef = Object();
            lightRef.getTransform()
                    .moveTo(glm::vec3(light.translation[0], light.translation[1], light.translation[2]))
                    .scaleTo(0.2);;
            lightRef.setGeometry<GeometryBuffer>(Path::inMemory("geo/1"), GeometryPreset::BOX, rb);
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
