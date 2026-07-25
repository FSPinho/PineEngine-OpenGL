#pragma once

#include <PineEngine/rendering/Shading/BaseShader/BaseShader.h>
#include <PineEngine/util/File/File.h>
#include <PineEngine/util/Path/Path.h>

namespace PineEngine {
    class GraphicShader : public BaseShader {
    public:
        explicit GraphicShader(
            const Path &path,
            const Path &vertexShaderPath,
            const Path &fragmentShaderPath,
            const RendererBackend &backend
        );
        ~GraphicShader() override;

    protected:
        uint32_t performShaderLoad() override;
        void performShaderUnload(uint32_t shadersId) override;

    private:
        File vertexShaderCodeFile;
        File fragmentShaderCodeFile;
    };
} // namespace PineEngine
