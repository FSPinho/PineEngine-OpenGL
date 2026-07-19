#include "Timer.h"

#include <PineEngine/util/Time/Time.h>

namespace PineEngine {
Timer::Timer() : startTime(Time::now()), lastFPSNotificationTime(Time::now()), frameSinceLastFPSNotification(0) {
    LOG_CONSTRUCTOR("Timer");
}

Timer::~Timer() {
    LOG_DESTRUCTOR("Timer");
}

float Timer::getElapsed() const {
    return Time::now() - this->startTime;
}

void Timer::trackFPS() {
    this->frameSinceLastFPSNotification += 1;
    const auto now = Time::now();
    const auto elapsed = now - this->lastFPSNotificationTime;
    if (elapsed >= 1.0) {
        auto fps = this->frameSinceLastFPSNotification / elapsed;
        LOG_METHOD(FORMAT("FPS {:.1f}", fps));

        this->frameSinceLastFPSNotification = 0;
        this->lastFPSNotificationTime = now;
    }
}
} // namespace PineEngine
