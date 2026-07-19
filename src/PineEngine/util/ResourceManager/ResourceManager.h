#pragma once

#include <PineEngine/util/SerialID/SerialID.h>
#include <PineEngine/util/Path/Path.h>
#include <PineEngine/util/Resource/Resource.h>
#include <PineEngine/util/ResourceHandler/ResourceHandler.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <iostream>

namespace PineEngine {
class ResourceManager {
  public:
    template <typename R, typename... Args> static ResourceHandler<R> load(const Path &path, Args &&...args) {
        static_assert(std::is_base_of_v<Resource, R>);

        std::string key = path.asString();

        // Create resource if it doesn't exist
        const bool exists = resources.contains(key);
        if (!exists) {
            std::unique_ptr<Resource> resource = std::make_unique<R>(path, args...);
            resources.emplace(key, std::move(resource));
            resources.at(key).get()->load();
        }

        // Update usage
        resourcesUsage[key]++;

        // Create the handler
        ResourceHandler handler(dynamic_cast<R *>(resources.at(key).get()));

        // Listen to handler destruction
        handler.addHandlerReleasedListener([key] {
            resourcesUsage[key]--;
            if (resourcesUsage[key] == 0) {
                resources.at(key).get()->unload();
                resources.erase(key);
            }
        });

        return handler;
    }

    static uint32_t getResourceUsage(const Path &path) { return resourcesUsage[path.asString()]; }

  private:
    inline static std::unordered_map<std::string, std::unique_ptr<Resource>> resources;
    inline static std::unordered_map<std::string, uint32_t> resourcesUsage;
};
} // namespace PineEngine
