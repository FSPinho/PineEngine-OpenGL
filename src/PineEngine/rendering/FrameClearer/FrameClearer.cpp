#include "FrameClearer.h"

namespace PineEngine {
FrameClearer::FrameClearer(RendererBackend &backend) : RendererComponent(backend) {
    LOG_CONSTRUCTOR("FrameClearer");
}

FrameClearer::~FrameClearer() {
    LOG_DESTRUCTOR("FrameClearer");
}

void FrameClearer::process() {
    this->backend.clearFrame();
}
} // namespace PineEngine
