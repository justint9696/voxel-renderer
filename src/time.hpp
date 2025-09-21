#pragma once

#include <chrono>

constexpr uint64_t MS_PER_SECOND = 1000;
constexpr uint64_t US_PER_SECOND = MS_PER_SECOND * 1000;
constexpr uint64_t NS_PER_SECOND = US_PER_SECOND * 1000;

constexpr uint64_t TICK_TARGET = 120;
constexpr uint64_t NS_PER_TICK = (NS_PER_SECOND / TICK_TARGET);

namespace util {
    namespace time {
        inline int64_t now() {
            auto now = std::chrono::system_clock::now().time_since_epoch();
            auto duration =
                std::chrono::duration_cast<std::chrono::nanoseconds>(now);
            return duration.count();
        }
    }
}
