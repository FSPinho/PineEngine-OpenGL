#pragma once

#include <vector>
#include <PineEngine/rendering/RendererComponent/RendererComponent.h>
#include <PineEngine/rendering/RendererBackend/RendererBackend.h>


namespace PineEngine {
    class Renderer {
    public:
        explicit Renderer(RendererBackend &context);
        ~Renderer();

        void process();

        template<typename C>
        void addComponent(C &component) {
            static_assert(std::is_base_of_v<RendererComponent, C>);
            this->components.push_back(component);
        }

        template<typename C>
        void removeComponent(C &component) {
            static_assert(std::is_base_of_v<RendererComponent, C>);
            auto it = std::find_if(this->components.begin(), this->components.end(), [&component](const auto &c) {
                return c.get() == component;
            });
            this->components.erase(it);
        }

    private:
        RendererBackend &context;
        std::vector<std::reference_wrapper<RendererComponent> > components;
    };
}
