#pragma once

#include <PineEngine/platform/Input/Input.h>
#include <PineEngine/platform/Platform/Platform.h>


namespace PineEngine {
    class InputManager {
    public:
        explicit InputManager(Platform &platform);
        ~InputManager();

        void process() const;
        bool isKeyPressed(InputKey key) const;

    private:
        Platform &platform;
    };
}
