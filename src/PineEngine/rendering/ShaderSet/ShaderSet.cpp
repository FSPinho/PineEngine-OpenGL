#include "ShaderSet.h"


namespace PineEngine {
    ShaderSet::ShaderSet(
        RendererBackend &backend,
        std::string &&vertexShaderCodePath_,
        std::string &&fragmentShaderCodePath_
    ) : RendererComponent(backend),
        vertexShaderCodeFile(vertexShaderCodePath_),
        vertexShaderCodeFileWatch(vertexShaderCodePath_),
        fragmentShaderCodeFile(fragmentShaderCodePath_),
        fragmentShaderCodeFileWatch(fragmentShaderCodePath_) {
        LOG_CONSTRUCTOR("ShaderSet");
    }

    ShaderSet::~ShaderSet() {
        LOG_DESTRUCTOR("ShaderSet");
        this->_unloadShaders();
    }

    void ShaderSet::setUniform(const std::string &name, const std::vector<float> &value) {
        this->floatUniforms.emplace(name, value);

        if (this->areShadersLoaded) {
            this->backend.setUniform(this->shadersId, name, value);
        }
    }

    void ShaderSet::setUniform(const std::string &name, const glm::mat4 &value) {
        this->matrixUniforms.emplace(name, value);

        if (this->areShadersLoaded) {
            this->backend.setUniform(this->shadersId, name, value);
        }
    }

    void ShaderSet::process() {
        this->_maybeReloadShaders();
        this->backend.prepareShadersForRendering(this->shadersId);
    }

    void ShaderSet::_maybeReloadShaders() {
        const auto hasVertexShaderChanged = this->vertexShaderCodeFileWatch.hasChanged();
        const auto hasFragmentShaderChanged = this->fragmentShaderCodeFileWatch.hasChanged();
        if (!hasVertexShaderChanged && !hasFragmentShaderChanged) return;
        this->_loadShaders();
    }

    void ShaderSet::_loadShaders() {
        const auto vertexShaderCode = this->vertexShaderCodeFile.readAsText();
        const auto fragmentShaderCode = this->fragmentShaderCodeFile.readAsText();

        this->_unloadShaders();
        this->shadersId = this->backend.createShaders(
            vertexShaderCode,
            fragmentShaderCode
        );

        for (const auto &[name, value]: this->floatUniforms) {
            this->backend.setUniform(this->shadersId, name, value);
        }

        this->areShadersLoaded = true;
    }

    void ShaderSet::_unloadShaders() {
        if (this->areShadersLoaded) {
            this->backend.deleteShaders(this->shadersId);
        }
    }
}
