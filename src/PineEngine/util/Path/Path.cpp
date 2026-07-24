#include "Path.h"

#include <stdexcept>

namespace PineEngine {
    Path::Path(const std::string &path) {
        this->_initializeParts(path);
    }

    std::string Path::getPrefix() const {
        return this->prefix;
    }

    std::string Path::asString(const bool includePrefix) const {
        std::string str;
        for (uint32_t i = 0; i < this->parts.size(); i++) {
            if (i > 0) {
                str += "/";
            }
            str += this->parts[i];
        }
        if (includePrefix && this->prefix.size()) {
            str = this->prefix + '/' + str;
        }
        return std::move(str);
    }

    std::string Path::asAbsolutePathString() const {
        return inDiskRootFolder + "/" + this->asString(false);
    }

    bool Path::operator==(const std::string &other) const {
        return this->asString() == other;
    }

    Path Path::operator/(const std::string &other) const {
        return Path(this->asString() + "/" + other);
    }

    Path Path::inMemory(const std::string &path) {
        return std::move(Path(std::string(CONFIG::IN_MEMORY_RESOURCE_PREFIX) + "/" + path));
    }

    Path Path::inDisk(const std::string &path) {
        return std::move(Path(std::string(CONFIG::IN_DISK_RESOURCE_PREFIX) + "/" + path));
    }

    void Path::setInDiskRootFolder(const std::string &path) {
        inDiskRootFolder = path;
    }

    void Path::_initializeParts(const std::string &path) {
        std::string part = "";
        bool haveSeemColon = false;
        bool haveSeemSlash = false;

        for (uint32_t i = 0; i < path.size(); i++) {
            if (path[i] == ':') {
                if (haveSeemSlash) {
                    throw std::runtime_error("Path prefix cannot contain slash!");
                }

                if (haveSeemColon) {
                    throw std::runtime_error("Path only supports one occurrence of ':' to define the prefix!");
                }
                haveSeemColon = true;

                this->prefix = path.substr(0, i + 2);
                if (!this->prefix.ends_with(":/")) {
                    throw std::runtime_error(FORMAT("Invalid path prefix: '{}'!", this->prefix));
                }

                part = "";
                i++;
            }

            if (path[i] == '/') {
                if (part.size()) {
                    this->parts.push_back(part);
                }
                haveSeemSlash = true;
                part = "";
            } else {
                part += path[i];
            }
        }
        if (part.size()) {
            this->parts.push_back(part);
        }
    }
} // namespace PineEngine