#include "Renderer.h"

namespace PineEngine {
    Renderer::Renderer(RendererBackend &backend) : backend(backend) {
        LOG_CONSTRUCTOR("Renderer");
    }

    Renderer::~Renderer() {
        LOG_DESTRUCTOR("Renderer");
    }

    void Renderer::clearFrame() {
        this->backend.clearFrame();
    }

    void Renderer::commitFrame() {
        this->backend.swapBuffers();
    }
} // namespace PineEngine
