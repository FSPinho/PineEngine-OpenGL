#pragma once

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
        this->_deleteShaders();
    }

    void ShaderSet::setUniform(const std::string &name, const std::vector<float> &value) {
        this->vecFloatUniforms.emplace_back(name, value);
    }

    void ShaderSet::process() {
        this->_loadShaders();
        this->backend.prepareShadersForRendering(this->shadersId);
    }

    void ShaderSet::_loadShaders() {
        const auto hasVertexShaderChanged = this->vertexShaderCodeFileWatch.hasChanged();
        const auto hasFragmentShaderChanged = this->fragmentShaderCodeFileWatch.hasChanged();
        if (!hasVertexShaderChanged && !hasFragmentShaderChanged) return;

        const auto vertexShaderCode = this->vertexShaderCodeFile.readAsText();
        const auto fragmentShaderCode = this->fragmentShaderCodeFile.readAsText();

        this->_deleteShaders();
        this->shadersId = this->backend.createShaders(
            vertexShaderCode,
            fragmentShaderCode
        );

        for (const auto &[name, value]: this->vecFloatUniforms) {
            this->backend.setUniform(this->shadersId, name, value);
        }

        this->areShadersLoaded = true;
    }

    void ShaderSet::_deleteShaders() {
        if (this->areShadersLoaded) {
            this->backend.deleteShaders(this->shadersId);
        }
    }
}
