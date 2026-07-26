#pragma once

#include <PineEngine/rendering/Shading/BaseShader/BaseShader.h>
#include <PineEngine/util/File/File.h>
#include <PineEngine/util/Path/Path.h>

namespace PineEngine {
    class ComputeShader : public BaseShader {
    public:
        explicit ComputeShader(const Path &path, const RendererBackend &backend);
        ~ComputeShader() override;

        void setInvocationCount(uint32_t xInvocationCount_, uint32_t yInvocationCount_);
        void bindTextureForCompute(uint32_t textureId, uint32_t attributeIndex);

        void executeCompute();

    protected:
        uint32_t performShaderLoad() override;
        void performShaderUnload(uint32_t shadersId) override;

    private:
        File computeShaderCodeFile;

        uint32_t xInvocationCount = 0;
        uint32_t yInvocationCount = 0;
    };
} // namespace PineEngine
