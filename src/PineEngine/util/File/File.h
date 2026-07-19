#pragma once

#include <string>

namespace PineEngine {
class File {
  public:
    explicit File(std::string path_);

    std::string readAsText();

  private:
    std::string path;
};
} // namespace PineEngine
