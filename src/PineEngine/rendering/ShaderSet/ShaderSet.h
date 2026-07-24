#pragma once

#include <PineEngine/rendering/RendererComponent/RendererComponent.h>
#include <PineEngine/util/File/File.h>
#include <PineEngine/util/FileWatch/FileWatch.h>
#include <PineEngine/util/Path/Path.h>
#include <PineEngine/util/Resource/Resource.h>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace PineEngine {
    class ShaderSet : public Resource, RendererComponent {
    public:
        explicit ShaderSet(const Path &path, RendererBackend &backend);
        ~ShaderSet() override;

        void setUniform(const std::string &name, const std::vector<uint32_t> &value);
        void setUniform(const std::string &name, const std::vector<float> &value);
        void setUniform(const std::string &name, const glm::mat4 &value);

        void performRendering() override;

    protected:
        void performLoad() override;
        void performUnload() override;

    private:
        uint32_t shadersId = 0;
        bool areShadersLoaded = false;

        File vertexShaderCodeFile;
        FileWatch vertexShaderCodeFileWatch;
        File fragmentShaderCodeFile;
        FileWatch fragmentShaderCodeFileWatch;

        std::unordered_map<std::string, std::vector<uint32_t> > intUniforms;
        std::unordered_map<std::string, std::vector<float> > floatUniforms;
        std::unordered_map<std::string, glm::mat4> matrixUniforms;

        void _loadShaders();
        void _unloadShaders();
        void _setUniforms();
    };
} // namespace PineEngine
