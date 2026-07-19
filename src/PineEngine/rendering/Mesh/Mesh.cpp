#include "Mesh.h"

namespace PineEngine {
Mesh::Mesh(RendererBackend &backend) : RendererComponent(backend) {
    LOG_CONSTRUCTOR("FrameClearer");
}

Mesh::~Mesh() {
    LOG_DESTRUCTOR("FrameClearer");
}

void Mesh::process() {
    this->backend.clearFrame();
}
} // namespace PineEngine
