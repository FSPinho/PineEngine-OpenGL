#pragma once

#include <PineEngine/util/SerialID/SerialID.h>
#include <PineEngine/rendering/RendererBackend/RendererBackend.h>


namespace PineEngine {
    class RendererComponent {
        friend class Renderer;

    public:
        explicit RendererComponent(RendererBackend &backend);

        virtual ~RendererComponent() = default;

        bool operator==(const RendererComponent &other) const;

    protected:
        RendererBackend &backend;
        virtual void process() = 0;


    private:
        ID id;
    };
}
