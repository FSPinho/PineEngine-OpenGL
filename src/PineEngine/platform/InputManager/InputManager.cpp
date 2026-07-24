#include "InputManager.h"

namespace PineEngine {
    InputManager::InputManager(Platform &platform) : platform(platform) {
        LOG_CONSTRUCTOR("InputManager");

        this->platform.addScrollListener([this](const double deltaX, const double deltaY) {
            this->currentMouseInput.wheelDeltaX = deltaX;
            this->currentMouseInput.wheelDeltaY = deltaY;
        });
    }

    InputManager::~InputManager() {
        LOG_DESTRUCTOR("InputManager");
    }

    void InputManager::preProcessMouseEvents(const Tick &tick) {
        this->currentMouseInput.isLeftButtonPressed = this->platform.isMouseLeftButtonPressed();
        this->currentMouseInput.isMiddleButtonPressed = this->platform.isMouseMiddleButtonPressed();
        this->currentMouseInput.isRightButtonPressed = this->platform.isMouseRightButtonPressed();

        auto [mouseX, mouseY] = this->platform.getMousePosition();

        if (this->didProcessMouseEventsOnce) {
            this->currentMouseInput.deltaX = mouseX - this->currentMouseInput.x;
            this->currentMouseInput.deltaY = mouseY - this->currentMouseInput.y;
        }

        this->currentMouseInput.x = mouseX;
        this->currentMouseInput.y = mouseY;

        this->didProcessMouseEventsOnce = true;
    }

    void InputManager::postProcessMouseEvents() {
        this->currentMouseInput.wheelDeltaX = 0.0;
        this->currentMouseInput.wheelDeltaY = 0.0;
    }

    MouseInput &InputManager::getMouseInput() {
        return this->currentMouseInput;
    }


    bool InputManager::isKeyPressed(const InputKey key) const {
        return this->platform.isKeyPressed(key);
    }
} // namespace PineEngine
