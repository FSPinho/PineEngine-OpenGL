#pragma once

#define CONFIG PineEngine::Config

namespace PineEngine::Config {
    constexpr int WINDOW_WIDTH = 1280;
    constexpr int WINDOW_HEIGHT = 720;
    constexpr auto WINDOW_TITLE = "Pine Engine";

    constexpr auto MEMORY_RESOURCE_PREFIX = "mem:/";
    constexpr auto EXTERNAL_RESOURCE_PREFIX = "src:/";
    constexpr auto SYSTEM_RESOURCE_PREFIX = "sys:/";
} // namespace PineEngine::Config
