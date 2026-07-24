#pragma once
#include <PineEngine/util/Log/Log.h>
#include <PineEngine/util/Resource/Resource.h>


namespace PineEngine {
    class ResourceManager;

    template<typename R>
        requires std::derived_from<R, Resource>
    class ResourceHandler {
    public:
        explicit ResourceHandler() : ResourceHandler(nullptr) {
        }

        explicit ResourceHandler(R *resource)
            : resource(resource),
              resourceKey(
                  resource != nullptr
                      ? resource->getPath().asString()
                      : "-undefined-"
              ) {
            if (this->resource != nullptr) {
                ResourceManager::notifyResourceAcquired(this->resourceKey);
            }
        }

        ResourceHandler(const ResourceHandler &other) : ResourceHandler(other.resource) {
        }

        ResourceHandler &operator=(const ResourceHandler &) = delete;

        ResourceHandler(ResourceHandler &&) noexcept = default;
        ResourceHandler &operator=(ResourceHandler &&) noexcept = default;

        ~ResourceHandler() {
            if (this->resource != nullptr) {
                ResourceManager::notifyResourceReleased(this->resourceKey);
            }
        }

        R *operator->() { return this->resource; }
        R &operator*() { return *this->resource; }

        explicit operator bool() const { return this->resource != nullptr; }

    private:
        R *resource;
        std::string resourceKey;
    };
} // namespace PineEngine
