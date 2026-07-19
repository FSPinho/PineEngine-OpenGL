#include "FileWatch.h"

#include <PineEngine/util/Time/Time.h>
#include <chrono>
#include <filesystem>

namespace PineEngine {
FileWatch::FileWatch(std::string path_) : path(std::move(path_)) {}

bool FileWatch::hasChanged() {
    auto now = Time::now();
    if (now - this->lastCheckTime < 1.0)
        return false;
    this->lastCheckTime = now;

    const double lastChangeTime = this->_getLastChangeTime();
    if (lastChangeTime > this->lastNotifiedChangeTime) {
        this->lastNotifiedChangeTime = lastChangeTime;
        return true;
    }

    return false;
}

double FileWatch::_getLastChangeTime() {
    const auto t = std::filesystem::last_write_time(this->path).time_since_epoch();
    return std::chrono::duration<double>(t).count();
}
} // namespace PineEngine
