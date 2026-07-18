#pragma once

#include <string>


namespace PineEngine {
    class Resource {
        friend class ResourceManager;

    protected:
        explicit Resource(std::string key);
        virtual ~Resource();

    private:
        std::string key;

        virtual void load() = 0;
        virtual void unload() = 0;

        void requestLoad();
        void requestUnload();
    };
}
