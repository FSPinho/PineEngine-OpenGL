#pragma once

#include <PineEngine/application/Objects/BaseObject/BaseObject.h>
#include <PineEngine/rendering/Light/Light.h>
#include <PineEngine/util/SerialID/SerialID.h>
#include <vector>

namespace PineEngine {
    class Scene {
    public:
        explicit Scene();
        ~Scene();

        Scene &addChild(std::unique_ptr<Scene> child);
        BaseObject &addChild(std::unique_ptr<BaseObject> child);
        PointLight &addChild(std::unique_ptr<PointLight> child);
        DirectionalLight &addChild(std::unique_ptr<DirectionalLight> child);

        std::vector<std::unique_ptr<BaseObject> > &getObjects();
        std::vector<std::unique_ptr<PointLight> > &getPointLights();
        std::vector<std::unique_ptr<DirectionalLight> > &getDirectionalLights();

    private:
        ID id;

        Scene *parent = nullptr;

        std::vector<ID> childrenScenes;
        std::vector<ID> childrenObjects;
        std::vector<ID> childrenDirectionalLights;

        std::vector<std::unique_ptr<Scene> > allChildrenScenes;
        std::vector<std::unique_ptr<BaseObject> > allChildrenObjects;
        std::vector<std::unique_ptr<PointLight> > allChildrenPointLights;
        std::vector<std::unique_ptr<DirectionalLight> > allChildrenDirectionalLights;
    };
} // namespace PineEngine
