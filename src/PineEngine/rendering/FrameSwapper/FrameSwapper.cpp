#include "FrameSwapper.h"


namespace PineEngine {
    FrameSwapper::FrameSwapper(RendererBackend &backend) : RendererComponent(backend) {
        LOG_CONSTRUCTOR("FrameSwapper");
    }

    FrameSwapper::~FrameSwapper() {
        LOG_DESTRUCTOR("FrameSwapper");
    }

    void FrameSwapper::process() {
        this->backend.swapBuffers();
    }
}
