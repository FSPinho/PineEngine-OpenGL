#include "Editor.h"

#include <PineEngine/util/Timer/Timer.h>


namespace PineEngine {
    Editor::Editor(Application &application) : application(application),
                                               cameraController(application.getCamera(),
                                                                application.getInputManager()) {
        LOG_CONSTRUCTOR("Editor");
    }

    Editor::~Editor() {
        LOG_DESTRUCTOR("Editor");
    }

    void Editor::mainLoop() {
        this->application.mainLoop([this](const Tick &tick) {
            this->cameraController.processEvents(tick);
        });
    }
}
