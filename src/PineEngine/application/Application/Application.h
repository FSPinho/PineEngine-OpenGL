#pragma once

#include <PineEngine/application/Scene/Scene.h>
#include <PineEngine/util/Path/Path.h>
#include <PineEngine/util/ResourceHandler/ResourceHandler.h>
#include <PineEngine/util/ResourceManager/ResourceManager.h>

namespace PineEngine {
class Application {
  public:
    explicit Application();
    ~Application();

  private:
    ResourceHandler<Scene> rootScene = ResourceManager::load<Scene>(Path::inMemory("scenes/_root"));
};
} // namespace PineEngine