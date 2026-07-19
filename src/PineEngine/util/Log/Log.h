#pragma once

#include <format>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#ifdef NDEBUG
#define LOG(...)
#else
#define LOG(...) PineEngine::Log::i() << __VA_ARGS__
#endif

#define LOG_CONSTRUCTOR(...) LOG("[+] " << __VA_ARGS__)
#define LOG_DESTRUCTOR(...) LOG("[-] " << __VA_ARGS__)
#define LOG_METHOD(...) LOG("--- " << __VA_ARGS__)

#define FORMAT(...) std::format(__VA_ARGS__)

namespace PineEngine {
class LogStream {
  public:
    LogStream &operator<<(std::string const &value) {
        this->buffer += value;
        return *this;
    }

    LogStream &operator<<(char const *value) {
        this->buffer += value;
        return *this;
    }

    LogStream &operator<<(int const value) {
        this->buffer += std::to_string(value);
        return *this;
    }

    template <typename T>
        requires std::floating_point<T>
    LogStream &operator<<(T const value) {
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(4) << value;
        this->buffer += ss.str();
        return *this;
    }

    ~LogStream() { std::cout << this->buffer << std::endl; }

  private:
    std::string buffer;
};

class Log {
  public:
    static LogStream i() { return LogStream{}; }
};
} // namespace PineEngine
