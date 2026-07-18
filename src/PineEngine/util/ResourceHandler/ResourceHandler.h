#pragma once

namespace PineEngine {
    template<typename R>
    class ResourceHandler {
    public:
        explicit ResourceHandler(R *resource) : resource(resource) {
        }

        ~ResourceHandler() {
        }

    private:
        R *resource;
    };
}
