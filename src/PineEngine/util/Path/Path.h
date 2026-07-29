#pragma once

#include <string>
#include <vector>
#include <filesystem>

namespace PineEngine {
    class Path {
    public:
        explicit Path(const std::string &path);

        [[nodiscard]] std::string getPrefix() const;
        [[nodiscard]] std::string asString(bool includePrefix = true) const;
        [[nodiscard]] std::string asAbsolutePathString() const;

        bool operator==(const std::string &other) const;
        Path operator/(const std::string &other) const;

        static Path MEMORY();
        static Path RESOURCE(const std::string &path);
        static Path SYSTEM(const std::string &path);

        inline static std::string resourceRootFolder;
        static void setResourceRootFolder(const std::string &path);

    private:
        std::string prefix;
        std::vector<std::string> parts;

        void _initializeParts(const std::string &path);
    };
} // namespace PineEngine