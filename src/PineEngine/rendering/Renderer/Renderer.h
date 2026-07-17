#pragma once

#include <vector>
#include <memory>
#include <PineEngine/rendering/RendererComponent/RendererComponent.h>
#include <PineEngine/rendering/RendererBackend/RendererBackend.h>


namespace PineEngine {
    class Renderer {
    public:
        explicit Renderer(RendererBackend &context);
        ~Renderer();

        void process();

        template<typename T, typename... Args>
        T &addComponent(Args &&... args) {
            static_assert(std::is_base_of_v<RendererComponent, T>);
            this->components.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
            return static_cast<T &>(*this->components.back());
        }

        template<typename T>
        void removeComponent(T &component) {
            static_assert(std::is_base_of_v<RendererComponent, T>);
            auto it = std::find_if(this->components.begin(), this->components.end(), [&component](const auto &c) {
                return c.get() == component;
            });
            this->components.erase(it);
        }

    private:
        RendererBackend &context;
        std::vector<std::unique_ptr<RendererComponent> > components;
    };
}
