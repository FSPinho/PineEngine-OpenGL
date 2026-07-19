#pragma once

#include <functional>

// Note: GLAD library has to be included before glfw
// ReSharper disable once CppUnusedIncludeDirective
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <PineEngine/platform/Input/Input.h>

namespace PineEngine {
class Platform {
  public:
    Platform();
    ~Platform();

    void addResizeListener(std::function<void(uint32_t, uint32_t)> &&listener);

    void mainLoop(const std::function<void()> &tick) const;

    GLADloadproc getOpenGLProcAddress();

    bool isKeyPressed(InputKey key);

    void swapBuffers();

    void requestStop();

  private:
    GLFWwindow *window = nullptr;
    void _initializeWindow();
    void _destroyWindow();

    std::vector<std::function<void(uint32_t, uint32_t)>> resizeListeners;
    void _notifyResizeListeners();
};
} // namespace PineEngine
