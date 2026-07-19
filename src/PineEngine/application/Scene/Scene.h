#pragma once

#include <PineEngine/application/Camera/Camera.h>
#include <PineEngine/application/Object/Object.h>
#include <PineEngine/util/Resource/Resource.h>
#include <PineEngine/util/ResourceHandler/ResourceHandler.h>
#include <vector>

namespace PineEngine {
class Scene : public Resource {
  public:
    explicit Scene(const Path &path);
    ~Scene() override;

    void addChild(ResourceHandler<Scene> &&child);
    void addChild(ResourceHandler<Object> &&child);

    void performRendering(const double &time, const Camera &camera);

  protected:
    void performLoad() override;
    void performUnload() override;

  private:
    std::vector<ResourceHandler<Scene>> childrenScenes;
    std::vector<ResourceHandler<Object>> childrenObjects;
};
} // namespace PineEngine