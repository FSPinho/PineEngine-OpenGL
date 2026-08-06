#include "Scene.h"

#include "PineEngine/application/Application/Application.h"

namespace PineEngine {
    Scene::Scene() : id(SerialID::generate()) {
        LOG_CONSTRUCTOR(FORMAT("Scene[{}]", this->id));
    }

    Scene::~Scene() {
        LOG_DESTRUCTOR(FORMAT("Scene[{}]", this->id));
    }

    Scene &Scene::addChild(std::unique_ptr<Scene> child) {
        if (this->parent != nullptr) {
            return this->parent->addChild(std::move(child));
        }
        this->allChildrenScenes.push_back(std::move(child));
        return *this->allChildrenScenes.back();
    }

    Object &Scene::addChild(std::unique_ptr<Object> child) {
        if (this->parent != nullptr) {
            return this->parent->addChild(std::move(child));
        }
        this->allChildrenObjects.push_back(std::move(child));
        return *this->allChildrenObjects.back();
    }

    PointLight &Scene::addChild(std::unique_ptr<PointLight> child) {
        if (this->parent != nullptr) {
            return this->parent->addChild(std::move(child));
        }
        this->allChildrenPointLights.push_back(std::move(child));
        return *this->allChildrenPointLights.back();
    }

    DirectionalLight &Scene::addChild(std::unique_ptr<DirectionalLight> child) {
        if (this->parent != nullptr) {
            return this->parent->addChild(std::move(child));
        }
        this->allChildrenDirectionalLights.push_back(std::move(child));
        return *this->allChildrenDirectionalLights.back();
    }

    std::vector<std::unique_ptr<Object> > &Scene::getObjects() {
        return this->allChildrenObjects;
    }

    std::vector<std::unique_ptr<PointLight> > &Scene::getPointLights() {
        return this->allChildrenPointLights;
    }

    std::vector<std::unique_ptr<DirectionalLight> > &Scene::getDirectionalLights() {
        return this->allChildrenDirectionalLights;
    }
} // namespace PineEngine
