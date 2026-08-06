#pragma once

#include <functional>
#include <utility>
#include <PineEngine/util/Resource/Resource.h>
#include <PineEngine/util/Log/Log.h>


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
            this->_notifyAcquire();
        }

        ResourceHandler(const ResourceHandler &other)
            : ResourceHandler(other.resource, other.onAcquire, other.onRelease) {
        }

        ResourceHandler &operator=(const ResourceHandler &other)  {
            if (this != &other) {
                this->_notifyRelease();
                this->resource = other.resource;
                this->resourceKey = other.resource != nullptr ? other.resource->getPath().asString() : "-undefined-";
                this->onAcquire = other.onAcquire;
                this->onRelease = other.onRelease;
                this->_notifyAcquire();
            }
            return *this;
        }

        ResourceHandler(ResourceHandler &&) noexcept = default;

        ResourceHandler &operator=(ResourceHandler &&) noexcept = default;

        ~ResourceHandler() {
            this->_notifyRelease();
        }

        R *operator->() { return this->resource; }
        R &operator*() { return *this->resource; }

        explicit operator bool() const { return this->resource != nullptr; }

    private:
        R *resource;
        std::string resourceKey;

        std::function<void(std::string key)> onAcquire;
        std::function<void(std::string key)> onRelease;

        void _notifyAcquire() {
            if (this->resource != nullptr && this->onAcquire != nullptr) {
                this->onAcquire(this->resourceKey);
            }
        }

        void _notifyRelease() {
            if (this->resource != nullptr && this->onRelease != nullptr) {
                this->onRelease(this->resourceKey);
            }
        }
    };
} // namespace PineEngine
