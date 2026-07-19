#pragma once

#include <PineEngine/rendering/RendererComponent/RendererComponent.h>

namespace PineEngine {
class Timer : public RendererComponent {
  public:
    explicit Timer(RendererBackend &backend);
    ~Timer() override;

    float getElapsed();
    void trackFPS();

  protected:
    void process() override;
    double startTime;
    double lastFPSNotificationTime;
    uint32_t frameSinceLastFPSNotification;
};
} // namespace PineEngine
