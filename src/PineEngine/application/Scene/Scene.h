#pragma once

#include <PineEngine/application/Object/Object.h>
#include <PineEngine/application/Light/Light.h>
#include <PineEngine/util/SerialID/SerialID.h>
#include <vector>

namespace PineEngine {
    class Scene {
    public:
        explicit Scene();
        ~Scene();

        void addChild(Scene &&child);
        void addChild(Object &&child);
        void addChild(PointLight &&child);
        void addChild(DirectionalLight &&child);

        std::vector<Object>& getObjects();
        std::vector<PointLight>& getPointLights();
        std::vector<DirectionalLight>& getDirectionalLights();

    private:
        ID id;

        Scene *parent = nullptr;

        std::vector<ID> childrenScenes;
        std::vector<ID> childrenObjects;
        std::vector<ID> childrenDirectionalLights;

        std::vector<Scene> allChildrenScenes;
        std::vector<Object> allChildrenObjects;
        std::vector<PointLight> allChildrenPointLights;
        std::vector<DirectionalLight> allChildrenDirectionalLights;
    };
} // namespace PineEngine
