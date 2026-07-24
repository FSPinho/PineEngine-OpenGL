#include "Renderer.h"

namespace PineEngine {
    Renderer::Renderer(RendererBackend &backend) : backend(backend) {
        LOG_CONSTRUCTOR("Renderer");
        this->backend.clearFrame();
    }

    Renderer::~Renderer() {
        LOG_DESTRUCTOR("Renderer");
    }

    void Renderer::startFrame() {
        this->backend.clearFrame();
    }

    void Renderer::commitFrame() {
        this->backend.swapBuffers();
    }
} // namespace PineEngine
