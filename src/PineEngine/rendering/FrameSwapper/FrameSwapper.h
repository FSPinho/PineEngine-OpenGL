#pragma once

#include <PineEngine/rendering/RendererComponent/RendererComponent.h>

namespace PineEngine {
class FrameSwapper : public RendererComponent {
  public:
    explicit FrameSwapper(RendererBackend &backend);
    ~FrameSwapper() override;

  protected:
    void process() override;
};
} // namespace PineEngine
