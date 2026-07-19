#pragma once

#include <PineEngine/rendering/RendererComponent/RendererComponent.h>

namespace PineEngine {
class Timer {
  public:
    Timer();
    ~Timer();

    float getElapsed() const;
    void trackFPS();

  protected:
    double startTime;
    double lastFPSNotificationTime;
    uint32_t frameSinceLastFPSNotification;
};
} // namespace PineEngine
