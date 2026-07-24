#pragma once

#include <PineEngine/rendering/RendererBackend/RendererBackend.h>

namespace PineEngine {
    class RendererComponent {
    public:
        explicit RendererComponent(RendererBackend &backend);
        virtual ~RendererComponent() = default;

        virtual void performRendering() = 0;

    protected:
        RendererBackend &backend;
    };
} // namespace PineEngine
