#pragma once

#include <functional>

// Note: GLAD library has to be included before glfw
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <PineEngine/platform/Input/Input.h>

namespace PineEngine {
    class Platform {
    public:
        Platform();
        ~Platform();

        void addResizeListener(std::function<void(uint32_t, uint32_t)> &&listener);
        void addScrollListener(std::function<void(double, double)> &&listener);

        void mainLoop(const std::function<void()> &onTick) const;

        GLADloadproc getOpenGLProcAddress();

        bool isKeyPressed(InputKey key) const;
        bool isMouseLeftButtonPressed() const;
        bool isMouseMiddleButtonPressed() const;
        bool isMouseRightButtonPressed() const;
        std::pair<double, double> getMousePosition() const;

        void swapBuffers() const;

        void requestStop() const;

    private:
        GLFWwindow *window = nullptr;
        void _initializeWindow();
        void _destroyWindow();

        std::vector<std::function<void(uint32_t, uint32_t)> > resizeListeners;
        void _notifyResizeListeners();

        std::vector<std::function<void(double, double)> > scrollListeners;
        void _notifyScrollListeners(double deltaX, double deltaY);
    };
} // namespace PineEngine
