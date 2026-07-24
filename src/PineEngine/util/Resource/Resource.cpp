#include "Resource.h"

namespace PineEngine {
    Resource::Resource(const Path &path) : path(path) {
        // ...
    }

    Resource::~Resource() {
        // ...
    }

    void Resource::load() {
        this->performLoad();
    }

    void Resource::unload() {
        this->performUnload();
    }

    const Path &Resource::getPath() {
        return this->path;
    }
} // namespace PineEngine
