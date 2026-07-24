#pragma once

#include <PineEngine/rendering/RendererComponent/RendererComponent.h>

namespace PineEngine {
    struct Tick {
        double elapsed = 0.0;
        double delta = 0.0;
    };

    class Timer {
    public:
        Timer();
        ~Timer();

        Tick getNextTick();
        void trackFPS();

    protected:
        double startTime;
        double lastTickNotificationTime;
        double lastFPSNotificationTime;
        uint32_t framesSinceLastFPSNotification;
    };
} // namespace PineEngine
