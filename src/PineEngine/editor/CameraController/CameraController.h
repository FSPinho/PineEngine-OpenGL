#pragma once

#include <PineEngine/util/Timer/Timer.h>
#include <PineEngine/application/Camera/Camera.h>
#include <PineEngine/platform/InputManager/InputManager.h>

namespace PineEngine {
    class CameraController {
    public:
        CameraController(Camera &camera, InputManager &inputManager);
        ~CameraController();

        void processEvents(const Tick &tick);

    private:
        Camera &camera;
        InputManager &inputManager;
    };
}
