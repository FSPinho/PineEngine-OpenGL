#include "File.h"

#include <fstream>
#include <sstream>
#include <stdexcept>


namespace PineEngine {
    File::File(std::string path_) : path(std::move(path_)) {
    }

    std::string File::readAsText() {
        const std::ifstream file(this->path);

        if (!file) throw std::runtime_error(FORMAT("Failed to load file {}", this->path));

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
}
