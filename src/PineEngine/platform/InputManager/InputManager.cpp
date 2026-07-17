#include "InputManager.h"

namespace PineEngine {
    InputManager::InputManager(Platform &platform) : platform(platform) {
        LOG_CONSTRUCTOR("InputManager");
    }

    InputManager::~InputManager() {
        LOG_DESTRUCTOR("InputManager");
    }

    void InputManager::process() const {
        // ...
    }

    bool InputManager::isKeyPressed(const InputKey key) const {
        return this->platform.isKeyPressed(key);
    }
}
