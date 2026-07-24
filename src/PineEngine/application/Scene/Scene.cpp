#include "Scene.h"

#include "PineEngine/application/Application/Application.h"

namespace PineEngine {
    Scene::Scene() : id(SerialID::generate()) {
        LOG_CONSTRUCTOR(FORMAT("Scene[{}]", this->id));
    }

    Scene::~Scene() {
        LOG_DESTRUCTOR(FORMAT("Scene[{}]", this->id));
    }

    void Scene::addChild(Scene &&child) {
        if (this->parent != nullptr) {
            this->parent->addChild(std::move(child));
        } else {
            this->allChildrenScenes.push_back(std::move(child));
        }
    }

    void Scene::addChild(Object &&child) {
        if (this->parent != nullptr) {
            this->parent->addChild(std::move(child));
        } else {
            this->allChildrenObjects.push_back(child);
        }
    }

    void Scene::addChild(PointLight &&child) {
        if (this->parent != nullptr) {
            this->parent->addChild(std::move(child));
        } else {
            this->allChildrenPointLights.push_back(child);
        }
    }

    std::vector<Object> &Scene::getObjects() {
        return this->allChildrenObjects;
    }

    std::vector<PointLight> &Scene::getPointLights() {
        return this->allChildrenPointLights;
    }
} // namespace PineEngine
