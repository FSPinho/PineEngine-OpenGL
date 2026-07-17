#include "Renderer.h"


namespace PineEngine {
    Renderer::Renderer(RendererBackend &context) : context(context) {
        LOG_CONSTRUCTOR("Renderer");
    }

    Renderer::~Renderer() {
        LOG_DESTRUCTOR("Renderer");
    }

    void Renderer::process() {
        for (const auto &component: this->components) {
            component->process();
        }
    }
}
