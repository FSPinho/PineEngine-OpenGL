#include "ComputeShader.h"

namespace PineEngine {
    ComputeShader::ComputeShader(const Path &path, const RendererBackend &backend)
        : BaseShader(path, backend),
          computeShaderCodeFile(path) {
        LOG_CONSTRUCTOR("ComputeShader");
    }

    ComputeShader::~ComputeShader() {
        LOG_DESTRUCTOR("ComputeShader");
    }

    void ComputeShader::setInvocationCount(const uint32_t xInvocationCount_, const uint32_t yInvocationCount_) {
        this->xInvocationCount = xInvocationCount_;
        this->yInvocationCount = yInvocationCount_;
    }

    void ComputeShader::bindTextureForCompute(const uint32_t textureId, uint32_t attributeIndex) {
        this->backend.bindTextureForCompute(textureId, attributeIndex);
    }

    void ComputeShader::executeCompute() {
        this->backend.executeComputeShader(this->shadersId, this->xInvocationCount, this->yInvocationCount);
        this->backend.waitComputeShader();
    }

    uint32_t ComputeShader::performShaderLoad() {
        const auto computeShaderCode = this->computeShaderCodeFile.readAsText();
        return this->backend.createComputeShader(computeShaderCode);
    }

    void ComputeShader::performShaderUnload(uint32_t shadersId) {
        this->backend.deleteShaders(this->shadersId);
    }
} // namespace PineEngine
