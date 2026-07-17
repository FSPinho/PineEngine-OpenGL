#pragma once

#include <string>
#include <PineEngine/rendering/RendererComponent/RendererComponent.h>


namespace PineEngine {
    class ShaderSet : public RendererComponent {
    public:
        explicit ShaderSet(
            RendererBackend &backend,
            std::string &&vertexShaderCode,
            std::string &&fragmentShaderCode
        );
        ~ShaderSet() override;

    protected:
        void process() override;

    private:
        uint32_t shadersId = 0;
        bool areShadersLoaded = false;
        std::string vertexShaderCode;
        std::string fragmentShaderCode;
    };
}
