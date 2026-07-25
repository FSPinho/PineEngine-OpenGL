#include "GraphicShader.h"

namespace PineEngine {
    GraphicShader::GraphicShader(
        const Path &path,
        const Path &vertexShaderPath,
        const Path &fragmentShaderPath,
        const RendererBackend &backend
    )
        : BaseShader(path, backend),
          vertexShaderCodeFile(vertexShaderPath),
          fragmentShaderCodeFile(fragmentShaderPath) {
        LOG_CONSTRUCTOR("GraphicShader");
    }

    GraphicShader::~GraphicShader() {
        LOG_DESTRUCTOR("GraphicShader");
    }

    uint32_t GraphicShader::performShaderLoad() {
        const auto vertexShaderCode = this->vertexShaderCodeFile.readAsText();
        const auto fragmentShaderCode = this->fragmentShaderCodeFile.readAsText();
        return this->backend.createShaders(vertexShaderCode, fragmentShaderCode);
    }

    void GraphicShader::performShaderUnload(const uint32_t shadersId) {
        this->backend.deleteShaders(shadersId);
    }
} // namespace PineEngine
