#pragma once

#include <functional>

// Note: GLAD library has to be included before glfw
// ReSharper disable once CppUnusedIncludeDirective
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <PineEngine/platform/Input/Input.h>


namespace PineEngine {
    class Platform {
    public:
        Platform();
        ~Platform();

        void mainLoop(const std::function<void()> &tick) const;

        GLADloadproc getOpenGLProcAddress();

        bool isKeyPressed(InputKey key);

        void swapBuffers();

        void requestStop();

    private:
        GLFWwindow *window = nullptr;
        void _initializeWindow();
        void _destroyWindow();
    };
}
