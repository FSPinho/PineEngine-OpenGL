#pragma once

#include <unordered_map>
#include <PineEngine/util/SerialID/SerialID.h>
#include <PineEngine/util/Resource/Resource.h>


namespace PineEngine {
    class ResourceManager {
    public:
        

    private:
        static std::unordered_map<ID, Resource> resources;
        static std::unordered_map<ID, uint32_t> resourcesUsage;
    };
}