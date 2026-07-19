#include "ShaderSet.h"

namespace PineEngine {
ShaderSet::ShaderSet(const Path &path, RendererBackend &backend)
    : Resource(path), RendererComponent(backend), vertexShaderCodeFile(path / "vertex.glsl"),
      vertexShaderCodeFileWatch(path / "vertex.glsl"), fragmentShaderCodeFile(path / "fragment.glsl"),
      fragmentShaderCodeFileWatch(path / "fragment.glsl") {
    LOG_CONSTRUCTOR("ShaderSet");
}

ShaderSet::~ShaderSet() {
    LOG_DESTRUCTOR("ShaderSet");
}

void ShaderSet::setUniform(const std::string &name, const std::vector<float> &value) {
    this->floatUniforms.insert_or_assign(name, value);
    this->_setUniforms();
}

void ShaderSet::setUniform(const std::string &name, const glm::mat4 &value) {
    this->matrixUniforms.insert_or_assign(name, value);
    this->_setUniforms();
}

void ShaderSet::performRendering() {
    this->backend.prepareShadersForRendering(this->shadersId);
}

void ShaderSet::performLoad() {
    this->_loadShaders();
}

void ShaderSet::performUnload() {
    this->_unloadShaders();
}

void ShaderSet::_loadShaders() {
    const auto vertexShaderCode = this->vertexShaderCodeFile.readAsText();
    const auto fragmentShaderCode = this->fragmentShaderCodeFile.readAsText();
    this->shadersId = this->backend.createShaders(vertexShaderCode, fragmentShaderCode);
    this->areShadersLoaded = true;
    this->_setUniforms();
}

void ShaderSet::_unloadShaders() {
    if (this->areShadersLoaded) {
        this->backend.deleteShaders(this->shadersId);
    }
}
void ShaderSet::_setUniforms() {
    if (this->areShadersLoaded) {
        for (const auto &[name, value] : this->floatUniforms) {
            this->backend.setUniform(this->shadersId, name, value);
        }
        for (const auto &[name, value] : this->matrixUniforms) {
            this->backend.setUniform(this->shadersId, name, value);
        }
    }
}
} // namespace PineEngine
