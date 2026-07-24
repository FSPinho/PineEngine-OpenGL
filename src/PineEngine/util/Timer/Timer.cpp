#include "Timer.h"

#include <PineEngine/util/Time/Time.h>

namespace PineEngine {
    Timer::Timer() : startTime(Time::now()), lastTickNotificationTime(0), lastFPSNotificationTime(Time::now()),
                     framesSinceLastFPSNotification(0) {
        LOG_CONSTRUCTOR("Timer");
    }

    Timer::~Timer() {
        LOG_DESTRUCTOR("Timer");
    }

    Tick Timer::getNextTick() {
        const double now = Time::now();
        const double elapsed = now - this->startTime;
        const double delta = now - this->lastTickNotificationTime;

        this->lastTickNotificationTime = now;

        return {elapsed, delta};
    }

    void Timer::trackFPS() {
        this->framesSinceLastFPSNotification += 1;
        const auto now = Time::now();
        const auto elapsed = now - this->lastFPSNotificationTime;
        if (elapsed >= 1.0) {
            auto fps = this->framesSinceLastFPSNotification / elapsed;
            LOG_METHOD(FORMAT("FPS {:.1f}", fps));

            this->framesSinceLastFPSNotification = 0;
            this->lastFPSNotificationTime = now;
        }
    }
} // namespace PineEngine
