#pragma once

#include <functional>
#include <utility>
#include <PineEngine/util/Log/Log.h>
#include <PineEngine/util/Resource/Resource.h>


namespace PineEngine {
    template<typename R>
        requires std::derived_from<R, Resource>
    class ResourceHandler {
    public:
        explicit ResourceHandler() : ResourceHandler(nullptr) {
        }

        explicit ResourceHandler(
            R *resource,
            std::function<void(std::string key)> onAcquire = nullptr,
            std::function<void(std::string key)> onRelease = nullptr
        )
            : resource(resource),
              resourceKey(
                  resource != nullptr
                      ? resource->getPath().asString()
                      : "-undefined-"
              ),
              onAcquire(std::move(onAcquire)),
              onRelease(std::move(onRelease)) {
            if (this->resource != nullptr && this->onAcquire != nullptr) {
                this->onAcquire(this->resourceKey);
            }
        }

        ResourceHandler(const ResourceHandler &other)
            : ResourceHandler(other.resource, other.onAcquire, other.onRelease) {
        }

        ResourceHandler &operator=(const ResourceHandler &) = delete;

        ResourceHandler(ResourceHandler &&) noexcept = default;

        ResourceHandler &operator=(ResourceHandler &&) noexcept = default;

        ~ResourceHandler() {
            if (this->resource != nullptr && this->onRelease != nullptr) {
                this->onRelease(this->resourceKey);
            }
        }

        R *operator->() { return this->resource; }
        R &operator*() { return *this->resource; }

        explicit operator bool() const { return this->resource != nullptr; }

    private:
        R *resource;
        std::string resourceKey;

        std::function<void(std::string key)> onAcquire;
        std::function<void(std::string key)> onRelease;
    };
} // namespace PineEngine
