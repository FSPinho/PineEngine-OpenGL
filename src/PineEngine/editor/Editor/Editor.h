#pragma once

#include<PineEngine/application/Application/Application.h>
#include<PineEngine/editor/CameraController/CameraController.h>

namespace PineEngine {
    class Editor {
    public:
        explicit Editor(Application &application);
        ~Editor();

        void mainLoop();

    private:
        Application &application;
        CameraController cameraController;
    };
}
