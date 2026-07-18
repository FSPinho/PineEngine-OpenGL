#pragma once

#include <string>
#include <unordered_map>
#include <PineEngine/util/SerialID/SerialID.h>
#include <PineEngine/util/Resource/Resource.h>
#include <PineEngine/util/ResourceHandler/ResourceHandler.h>


namespace PineEngine {
    class ResourceManager {
    public:
        template<typename R, typename... Args>
        static ResourceHandler<R> load(const std::string&& key, Args&&... args) {
            static_assert(std::is_base_of_v<Resource, R>);
            ResourceManager::resources.emplace("", args...);
            return ResourceHandler(ResourceManager::resources.at(key).get());
        }

    private:
        static std::unordered_map<std::string, std::unique_ptr<Resource>> resources;
        static std::unordered_map<std::string, uint32_t> resourcesUsage;
    };
}