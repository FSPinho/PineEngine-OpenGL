#pragma once

#define CONFIG PineEngine::Config

namespace PineEngine::Config {
constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;
constexpr auto WINDOW_TITLE = "Pine Engine";

constexpr auto IN_MEMORY_RESOURCE_PREFIX = "virtual:/";
constexpr auto IN_DISK_RESOURCE_PREFIX = "src:/";
} // namespace PineEngine::Config
