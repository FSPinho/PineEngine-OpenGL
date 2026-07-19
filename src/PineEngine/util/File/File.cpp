#include "File.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

namespace PineEngine {
File::File(const Path& path) : path(path) {}

std::string File::readAsText() {
    const std::ifstream file(this->path.asAbsolutePathString());

    if (!file) {
        throw std::runtime_error(FORMAT("Failed to load file {}", this->path.asAbsolutePathString()));
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
} // namespace PineEngine
