#include <PineEngine/application/Application/Application.h>
#include <PineEngine/util/Time/Time.h>
#include <iostream>

using namespace PineEngine;

int main() {
    try {
        Path::setInDiskRootFolder("../examples/Basic");

        Application application;

        // Add box
        auto box = ResourceManager::load<GeometryBuffer>(Path::inMemory("geometry/1"),
                                                         GeometryPreset::BOX,
                                                         application.getRendererBackend());
        auto object = ResourceManager::load<Object>(Path::inMemory("objects/1"));
        object->setGeometry(std::move(box));
        application.getRootScene()->addChild(std::move(object));

        application.mainLoop();

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
