#pragma once

#include <PineEngine/rendering/RendererBackend/RendererBackend.h>

namespace PineEngine {
    class Renderer {
    public:
        explicit Renderer(RendererBackend &backend);
        ~Renderer();

        void startFrame();
        void commitFrame();

    private:
        RendererBackend &backend;
    };
} // namespace PineEngine
