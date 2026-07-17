#include "Platform.h"

#include <unordered_map>
#include <stdexcept>


namespace PineEngine {
    Platform::Platform() {
        this->_initializeWindow();
        LOG_CONSTRUCTOR("Platform");
    }

    Platform::~Platform() {
        this->_destroyWindow();
        LOG_DESTRUCTOR("Platform");
    }

    void Platform::mainLoop(const std::function<void()> &tick) const {
        while (!glfwWindowShouldClose(this->window)) {
            tick();
            glfwPollEvents();
        }
    }

    GLADloadproc Platform::getOpenGLProcAddress() {
        return reinterpret_cast<GLADloadproc>(glfwGetProcAddress);
    }

    bool Platform::isKeyPressed(const InputKey key) {
        static std::unordered_map<InputKey, int> keyMap = {
            {InputKey::ESCAPE, GLFW_KEY_ESCAPE}
        };

        if (!keyMap.contains(key)) {
            throw std::runtime_error("Unmapped key!");
        }

        const int key_ = keyMap.at(key);
        return glfwGetKey(this->window, key_) == GLFW_PRESS;
    }

    void Platform::swapBuffers() {
        glfwSwapBuffers(this->window);
    }

    void Platform::requestStop() {
        glfwSetWindowShouldClose(this->window, true);
    }

    void Platform::_initializeWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        this->window = glfwCreateWindow(
            CONFIG::WINDOW_WIDTH,
            CONFIG::WINDOW_HEIGHT,
            CONFIG::WINDOW_TITLE,
            nullptr, nullptr
        );
        if (this->window == nullptr) {
            throw std::runtime_error("Failed to initialize window!");
        }
        glfwMakeContextCurrent(this->window);
        glfwSetFramebufferSizeCallback(
            this->window,
            [](GLFWwindow *window, const int width, const int height) {
                LOG("Window resized to width=" << width << ", height=" << height);
                // ... TODO
            }
        );
    }

    void Platform::_destroyWindow() {
        glfwDestroyWindow(this->window);
        glfwTerminate();
    }
}
