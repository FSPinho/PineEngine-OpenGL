#include "RendererComponent.h"


namespace PineEngine {
    RendererComponent::RendererComponent(RendererBackend &backend) : backend(backend), id(SerialID::generate()) {
    }

    bool RendererComponent::operator==(const RendererComponent &other) const {
        return this->id == other.id;
    }
}
