#include "Application.h"

namespace PineEngine {
Application::Application() {
    LOG_CONSTRUCTOR("Application");
}

Application::~Application() {
    LOG_DESTRUCTOR("Application");
}
} // namespace PineEngine
