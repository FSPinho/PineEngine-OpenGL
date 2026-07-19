#pragma once

#include <PineEngine/util/Path/Path.h>
#include <string>

namespace PineEngine {
class File {
  public:
    explicit File(const Path &path);

    std::string readAsText();

  private:
    Path path;
};
} // namespace PineEngine
