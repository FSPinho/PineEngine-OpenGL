#pragma once

#include <string>

namespace PineEngine {
class FileWatch {
  public:
    explicit FileWatch(std::string path_);

    bool hasChanged();

  private:
    std::string path;
    double lastCheckTime = 0.0f;
    double lastNotifiedChangeTime = 0.0f;

    double _getLastChangeTime();
};
} // namespace PineEngine
