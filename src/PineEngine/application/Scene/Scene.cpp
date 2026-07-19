#include "Scene.h"

namespace PineEngine {
Scene::Scene(const Path &path) : Resource(path) {
    LOG_CONSTRUCTOR(FORMAT("Scene[{}]", this->getPath().asString()));
}

Scene::~Scene() {
    LOG_DESTRUCTOR(FORMAT("Scene[{}]", this->getPath().asString()));
}

void Scene::addChild(ResourceHandler<Scene> &&child) {
    this->childrenScenes.push_back(std::move(child));
}

void Scene::addChild(ResourceHandler<Object> &&child) {
    this->childrenObjects.push_back(std::move(child));
}
void Scene::performRendering(const double &time, const Camera &camera) {
    for (auto& object : this->childrenObjects) {
        object->performRendering(time, camera);
    }

    for (auto& scene : this->childrenScenes) {
        scene->performRendering(time, camera);
    }
}

void Scene::performLoad() {}

void Scene::performUnload() {}
} // namespace PineEngine
