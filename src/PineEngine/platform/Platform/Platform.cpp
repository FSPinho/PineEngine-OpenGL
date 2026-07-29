#include "Platform.h"

#include <stdexcept>
#include <unordered_map>

namespace PineEngine {
    Platform::Platform() {
        this->_initializeWindow();
        LOG_CONSTRUCTOR("Platform");
    }

    Platform::~Platform() {
        this->_destroyWindow();
        LOG_DESTRUCTOR("Platform");
    }

    void Platform::addResizeListener(std::function<void(uint32_t, uint32_t)> &&listener) {
        this->resizeListeners.emplace_back(std::move(listener));
        this->_notifyResizeListeners();
    }

    void Platform::addScrollListener(std::function<void(double, double)> &&listener) {
        this->scrollListeners.emplace_back(std::move(listener));
    }

    void Platform::mainLoop(const std::function<void()> &onTick) const {
        while (!glfwWindowShouldClose(this->window)) {
            glfwPollEvents();
            onTick();
        }
    }

    GLADloadproc Platform::getOpenGLProcAddress() {
        return reinterpret_cast<GLADloadproc>(glfwGetProcAddress);
    }

    bool Platform::isKeyPressed(const InputKey key) const {
        static std::unordered_map<InputKey, std::vector<int> > keyMap = {
            {InputKey::SHIFT, {GLFW_KEY_LEFT_SHIFT, GLFW_KEY_RIGHT_SHIFT}},
            {InputKey::CONTROL, {GLFW_KEY_LEFT_CONTROL, GLFW_KEY_RIGHT_CONTROL}},
            {InputKey::ESCAPE, {GLFW_KEY_ESCAPE}},
            {InputKey::A, {GLFW_KEY_A}},
            {InputKey::D, {GLFW_KEY_D}},
            {InputKey::S, {GLFW_KEY_S}},
            {InputKey::W, {GLFW_KEY_W}},
        };

        if (!keyMap.contains(key)) {
            throw std::runtime_error("Unmapped key!");
        }

        const auto &mappedKeys = keyMap.at(key);
        for (const auto &key_: mappedKeys) {
            if (glfwGetKey(this->window, key_) == GLFW_PRESS) return true;
        }
        return false;
    }

    bool Platform::isMouseLeftButtonPressed() const {
        return glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    }

    bool Platform::isMouseMiddleButtonPressed() const {
        return glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS;
    }

    bool Platform::isMouseRightButtonPressed() const {
        return glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
    }

    std::pair<double, double> Platform::getMousePosition() const {
        double x, y;
        glfwGetCursorPos(this->window, &x, &y);
        return {x, y};
    }

    std::pair<uint32_t, uint32_t> Platform::getSurfaceSize() const {
        int width;
        int height;
        glfwGetWindowSize(this->window, &width, &height);
        return {width, height};
    }

    void Platform::swapBuffers() const {
        glfwSwapBuffers(this->window);
    }

    void Platform::requestStop() const {
        glfwSetWindowShouldClose(this->window, true);
    }

    void Platform::_initializeWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_ALPHA_BITS, 8);

        this->window =
                glfwCreateWindow(CONFIG::WINDOW_WIDTH, CONFIG::WINDOW_HEIGHT, CONFIG::WINDOW_TITLE, nullptr, nullptr);
        if (this->window == nullptr) {
            throw std::runtime_error("Failed to initialize window!");
        }
        glfwMakeContextCurrent(this->window);
        glfwSetWindowUserPointer(this->window, this);

        glfwSetFramebufferSizeCallback(this->window, [](GLFWwindow *window, const int width, const int height) {
            const auto p = static_cast<Platform *>(glfwGetWindowUserPointer(window));
            p->_notifyResizeListeners();
        });
        this->_notifyResizeListeners();

        glfwSetScrollCallback(this->window, [](GLFWwindow *window, const double deltaX, const double deltaY) {
            const auto p = static_cast<Platform *>(glfwGetWindowUserPointer(window));
            p->_notifyScrollListeners(deltaX, deltaY);
        });
    }

    void Platform::_destroyWindow() {
        glfwDestroyWindow(this->window);
        glfwTerminate();
    }

    void Platform::_notifyResizeListeners() {
        int width;
        int height;
        glfwGetWindowSize(this->window, &width, &height);
        for (const auto &listener: this->resizeListeners) {
            listener(width, height);
        }
    }

    void Platform::_notifyScrollListeners(const double deltaX, const double deltaY) {
        for (const auto &listener: this->scrollListeners) {
            listener(deltaX, deltaY);
        }
    }
} // namespace PineEngine
