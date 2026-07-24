#pragma once

namespace PineEngine {
    enum class InputKey { SHIFT, CONTROL, ESCAPE, A, D, S, W };

    struct MouseInput {
        bool isLeftButtonPressed = false;
        bool isMiddleButtonPressed = false;
        bool isRightButtonPressed = false;

        double x = 0;
        double y = 0;
        double deltaX = 0;
        double deltaY = 0;
        double wheelDeltaX = 0.0;
        double wheelDeltaY = 0.0;
    };
}