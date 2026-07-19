#pragma once

#include <PineEngine/rendering/RendererComponent/RendererComponent.h>

namespace PineEngine {
class Mesh : public RendererComponent {
  public:
    explicit Mesh(RendererBackend &backend);
    ~Mesh() override;

  protected:
    void process() override;
};
} // namespace PineEngine
