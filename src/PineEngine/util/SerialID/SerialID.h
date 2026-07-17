#pragma once

#include <cstdint>


namespace PineEngine {
    using ID = uint64_t;

    class SerialID {
    public:
        static ID generate() {
            return ++SerialID::lastGeneratedId;
        }

    private:
        static ID lastGeneratedId;
    };
}
