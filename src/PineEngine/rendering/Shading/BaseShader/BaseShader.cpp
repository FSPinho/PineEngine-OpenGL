#include "BaseShader.h"

namespace PineEngine {
    BaseShader::BaseShader(const Path &path, const RendererBackend &backend)
        : Resource(path), backend(backend) {
    }

    BaseShader::~BaseShader() = default;

    void BaseShader::prepareForRendering() {
        this->backend.prepareShadersForRendering(this->shadersId);
    }

    void BaseShader::setUniform(const std::string &name, const std::vector<uint32_t> &value) {
        this->backend.setUniform(this->shadersId, name, value);
    }

    void BaseShader::setUniform(const std::string &name, const std::vector<float> &value) {
        this->backend.setUniform(this->shadersId, name, value);
    }

    void BaseShader::setUniform(const std::string &name, const glm::mat4 &value) {
        this->backend.setUniform(this->shadersId, name, value);
    }

    void BaseShader::setUniformTexture(const std::string &name, const uint32_t textureId, bool multisampled) {
        this->backend.setUniformTexture(this->shadersId, name, textureId, multisampled);
    }

    void BaseShader::performLoad() {
        this->shadersId = this->performShaderLoad();
        this->areShadersLoaded = true;
    }

    void BaseShader::performUnload() {
        if (this->areShadersLoaded) {
            this->performShaderUnload(this->shadersId);
        }
    }
} // namespace PineEngine
