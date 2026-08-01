#pragma once

#include <PineEngine/rendering/RendererBackend/RendererBackend.h>
#include <PineEngine/util/Path/Path.h>
#include <PineEngine/util/Resource/Resource.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace PineEngine {
    class BaseShader : public Resource {
    public:
        explicit BaseShader(const Path &path, const RendererBackend &backend);
        ~BaseShader() override;

        virtual void prepareForRendering();

        void setUniform(const std::string &name, const std::vector<uint32_t> &value);
        void setUniform(const std::string &name, const std::vector<float> &value);
        void setUniform(const std::string &name, const glm::vec3 &value);
        void setUniform(const std::string &name, const glm::mat4 &value);
        void setUniformTexture(const std::string &name, uint32_t textureId, bool multisampled = false, uint32_t attributeIndex = 0);
        void setUniformCubeMapTexture(const std::string &name, uint32_t textureId);

    protected:
        RendererBackend backend;
        uint32_t shadersId = 0;

        void performLoad() override;
        void performUnload() override;

        virtual uint32_t performShaderLoad() = 0;
        virtual void performShaderUnload(uint32_t shadersId) = 0;

    private:
        bool areShadersLoaded = false;
    };
} // namespace PineEngine
