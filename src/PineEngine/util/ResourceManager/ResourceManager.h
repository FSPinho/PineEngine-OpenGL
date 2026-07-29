#pragma once

#include <PineEngine/util/SerialID/SerialID.h>
#include <PineEngine/util/Path/Path.h>
#include <PineEngine/util/Resource/Resource.h>
#include <PineEngine/util/ResourceHandler/ResourceHandler.h>
#include <PineEngine/util/Log/Log.h>
#include <memory>
#include <string>
#include <typeinfo>
#include <unordered_map>

namespace PineEngine {
    class ResourceManager {
    public:
        template<typename R, typename... Args>
        static ResourceHandler<R> load(const Path &path, const Args &... args) {
            static_assert(std::is_base_of_v<Resource, R>);
            std::string key = path.asString();

            try {
                // Create resource if it doesn't exist
                const bool exists = resources.contains(key);
                if (!exists) {
                    std::unique_ptr<Resource> resource = std::make_unique<R>(path, args...);
                    resources.emplace(key, std::move(resource));
                    resources.at(key).get()->load();
                }
            } catch (const std::exception &e) {
                throw std::runtime_error(FORMAT("Unable to load resource \"{}\": {}", key, e.what()));
            }

            try {
                dynamic_cast<R *>(resources.at(key).get());
            } catch (const std::exception &e) {
                throw std::runtime_error(FORMAT(
                    "Unable to cast resource key \"{}\" from type {}: {}",
                    key, typeid(R).name(), e.what()
                ));
            }

            try {
                // Create the handler
                ResourceHandler handler(
                    dynamic_cast<R *>(resources.at(key).get()),
                    [](const std::string &key_) {
                        notifyResourceAcquired(key_);
                    },
                    [](const std::string &key_) {
                        notifyResourceReleased(key_);
                    }
                );
                return std::move(handler);
            } catch (const std::exception &e) {
                throw std::runtime_error(FORMAT("Unable to create resource handler for \"{}\": {}", key, e.what()));
            }
        }

        static uint32_t getResourceUsage(const Path &path) { return resourcesUsage[path.asString()]; }

        static void notifyResourceAcquired(const std::string &key) {
            try {
                if (!resources.contains(key)) {
                    return;
                }
                resourcesUsage[key]++;
            } catch (const std::exception &e) {
                throw std::runtime_error(FORMAT("Unable to notify resource acquiring for \"{}\": {}", key, e.what()));
            }
        }

        static void notifyResourceReleased(const std::string &key) {
            try {
                if (!resources.contains(key)) {
                    return;
                }

                if (resourcesUsage[key] == 0) {
                    return;
                }

                resourcesUsage[key]--;
                if (resourcesUsage[key] == 0) {
                    resources.at(key).get()->unload();
                    resources.erase(key);
                }
            } catch (const std::exception &e) {
                throw std::runtime_error(FORMAT("Unable to notify resource release for \"{}\": {}", key, e.what()));
            }
        }

    private:
        inline static std::unordered_map<std::string, std::unique_ptr<Resource> > resources;
        inline static std::unordered_map<std::string, uint32_t> resourcesUsage;
    };
} // namespace PineEngine
