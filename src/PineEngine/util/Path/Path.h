#pragma once

#include <string>
#include <vector>

namespace PineEngine {
    class Path {
    public:
        explicit Path(const std::string &path);

        [[nodiscard]] std::string getPrefix() const;
        [[nodiscard]] std::string asString(bool includePrefix = true) const;
        [[nodiscard]] std::string asAbsolutePathString() const;

        bool operator==(const std::string &other) const;
        Path operator/(const std::string &other) const;

        static Path inMemory();
        static Path inDisk(const std::string &path);

        inline static std::string inDiskRootFolder;
        static void setInDiskRootFolder(const std::string &path);

    private:
        std::string prefix;
        std::vector<std::string> parts;

        void _initializeParts(const std::string &path);
    };
} // namespace PineEngine