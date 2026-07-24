#include "CameraController.h"
#include <PineEngine/platform/Input/Input.h>

namespace PineEngine {
    CameraController::CameraController(Camera &camera, InputManager &inputManager) : camera(camera),
        inputManager(inputManager) {
        LOG_CONSTRUCTOR("CameraController");
    }

    CameraController::~CameraController() {
        LOG_DESTRUCTOR("CameraController");
    }

    void CameraController::processEvents(const Tick &tick) {
        const auto &mouseInput = this->inputManager.getMouseInput();
        this->camera.move(mouseInput.wheelDeltaY, 0.0f);
        if (mouseInput.isMiddleButtonPressed) {
            this->camera.rotate(mouseInput.deltaX * -0.003, mouseInput.deltaY * 0.003);
        }
    }
}
