#pragma once

#include <string>
#include <vector>

namespace PineEngine {
class Path {
  public:
    explicit Path(const std::string &path);

    std::string getPrefix() const;
    std::string asString() const;

    bool operator==(const std::string &other) const;

    static Path inMemory(const std::string &path);
    static Path inDisk(const std::string &path);

  private:
    std::string prefix;
    std::vector<std::string> parts;

    void _initializeParts(const std::string &path);
};
} // namespace PineEngine