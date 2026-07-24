#pragma once

#include <PineEngine/util/Timer/Timer.h>
#include <PineEngine/platform/Input/Input.h>
#include <PineEngine/platform/Platform/Platform.h>

namespace PineEngine {
    class InputManager {
    public:
        explicit InputManager(Platform &platform);
        ~InputManager();

        void preProcessMouseEvents(const Tick &tick);
        void postProcessMouseEvents();
        MouseInput &getMouseInput();
        bool isKeyPressed(InputKey key) const;

    private:
        Platform &platform;

        bool didProcessMouseEventsOnce = false;
        MouseInput currentMouseInput;
    };
} // namespace PineEngine
