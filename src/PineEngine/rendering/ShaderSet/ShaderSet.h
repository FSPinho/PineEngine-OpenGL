#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include <PineEngine/util/File/File.h>
#include <PineEngine/util/FileWatch/FileWatch.h>
#include <PineEngine/rendering/RendererComponent/RendererComponent.h>


namespace PineEngine {
    class ShaderSet : public RendererComponent {
    public:
        explicit ShaderSet(
            RendererBackend &backend,
            std::string &&vertexShaderCodePath_,
            std::string &&fragmentShaderCodePath_
        );
        ~ShaderSet() override;

        void setUniform(const std::string &name, const std::vector<float> &value);
        void setUniform(const std::string &name, const glm::mat4 &value);

    protected:
        void process() override;

    private:
        uint32_t shadersId = 0;
        bool areShadersLoaded = false;

        File vertexShaderCodeFile;
        FileWatch vertexShaderCodeFileWatch;
        File fragmentShaderCodeFile;
        FileWatch fragmentShaderCodeFileWatch;

        std::unordered_map<std::string, std::vector<float>> floatUniforms;
        std::unordered_map<std::string, glm::mat4> matrixUniforms;

        void _maybeReloadShaders();
        void _loadShaders();
        void _unloadShaders();
    };
}
