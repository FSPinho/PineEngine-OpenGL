#pragma once

#include "ShaderSet.h"


namespace PineEngine {
    ShaderSet::ShaderSet(
        RendererBackend &backend,
        std::string &&vertexShaderCode,
        std::string &&fragmentShaderCode
    ) : RendererComponent(backend),
        vertexShaderCode(std::move(vertexShaderCode)),
        fragmentShaderCode(std::move(fragmentShaderCode)) {
        this->shadersId = this->backend.createShaders(
            this->vertexShaderCode,
            this->fragmentShaderCode
        );
        this->areShadersLoaded = true;

        LOG_CONSTRUCTOR("ShaderSet");
    }

    ShaderSet::~ShaderSet() {
        LOG_DESTRUCTOR("ShaderSet");

        if (this->areShadersLoaded) {
            this->backend.deleteShaders(this->shadersId);
        }
    }

    void ShaderSet::process() {
        this->backend.prepareShadersForRendering(this->shadersId);
    }
}
