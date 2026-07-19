#pragma once

#include <PineEngine/rendering/RendererComponent/RendererComponent.h>

namespace PineEngine {
class FrameClearer : public RendererComponent {
  public:
    explicit FrameClearer(RendererBackend &backend);
    ~FrameClearer() override;

  protected:
    void process() override;
};
} // namespace PineEngine
