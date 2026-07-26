#pragma once

#include <PineEngine/rendering/RendererBackend/RendererBackend.h>
#include <PineEngine/util/Resource/Resource.h>

namespace PineEngine {
    class VolumeBuffer : public Resource {
    public:
        explicit VolumeBuffer(const Path &path, uint32_t resolution, const RendererBackend& backend);
        ~VolumeBuffer() override;

        uint32_t getResolution() const;

        void prepareForCompute(uint32_t bufferAttributeIndex);

    protected:
        void performLoad() override;
        void performUnload() override;

    private:
        uint32_t resolution;
        RendererBackend backend;

        uint32_t bufferId = 0;
        bool isLoaded = false;
    };
}
