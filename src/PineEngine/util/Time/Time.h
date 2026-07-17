#pragma once

#include <chrono>


namespace PineEngine {
    class Time {
    public:
        static double now() {
            return std::chrono::duration<double>(
                std::chrono::steady_clock::now().time_since_epoch()
            ).count();
        }
    };
}
