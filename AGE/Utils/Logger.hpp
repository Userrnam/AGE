#pragma once

#include <string>


namespace age {

struct Logger {
    static void info(const char* format, ...);
    static void warn(const char* format, ...);
    static void error(const char* format, ...);
    static void print(const char* format, ...);
};

} // namespace age
