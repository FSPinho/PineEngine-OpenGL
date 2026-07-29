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
        const float dist = std::sqrt(glm::length(this->camera.getTranslation() - this->camera.getTarget()) * 0.1f);
        this->camera.move(static_cast<float>(mouseInput.wheelDeltaY) * dist, 0.0f);
        if (mouseInput.isMiddleButtonPressed) {
            this->camera.rotate(static_cast<float>(mouseInput.deltaX) * -0.003f, static_cast<float>(mouseInput.deltaY) * 0.003f);
        }
    }
}
