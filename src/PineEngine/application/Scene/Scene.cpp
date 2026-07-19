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
void Scene::performRendering() {
    for (auto& object : this->childrenObjects) {
        object->performRendering();
    }

    for (auto& scene : this->childrenScenes) {
        scene->performRendering();
    }
}

void Scene::performLoad() {}

void Scene::performUnload() {}
} // namespace PineEngine
