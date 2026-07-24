#pragma once

#include <PineEngine/util/Path/Path.h>

namespace PineEngine {
    class FileWatch {
    public:
        explicit FileWatch(const Path &path);

        bool hasChanged();

    private:
        Path path;
        double lastCheckTime = 0.0f;
        double lastNotifiedChangeTime = 0.0f;

        double _getLastChangeTime();
    };
} // namespace PineEngine
