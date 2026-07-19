#pragma once

#include <functional>
#include <vector>

namespace PineEngine {
template <typename R> class ResourceHandler {
  public:
    explicit ResourceHandler(R *resource) : resource(resource) {}

    // Block copy
    ResourceHandler(const ResourceHandler &) = delete;
    ResourceHandler &operator=(const ResourceHandler &) = delete;
    ResourceHandler(ResourceHandler &&) = default;
    ResourceHandler &operator=(ResourceHandler &&) = default;

    ~ResourceHandler() {
        for (const auto &listener : this->releaseListeners) {
            listener();
        }
    }

    R *operator->() { return this->resource; }

    void addHandlerReleasedListener(const std::function<void()> &&listener) {
        this->releaseListeners.push_back(std::move(listener));
    }

  private:
    R *resource;
    std::vector<std::function<void()>> releaseListeners;
};
} // namespace PineEngine
