#include <stdarg.h>
#include <stdio.h>

#include "Logger.hpp"


namespace age {

void Logger::info(const char* format, ...) {
    va_list args;

    printf("Info: ");

    va_start(args, format);

    vprintf(format, args);

    va_end(args);

    putchar('\n');
}

void Logger::warn(const char* format, ...) {
    va_list args;

    printf("\e[0;33mWarning: \e[0m");

    va_start(args, format);

    vprintf(format, args);

    va_end(args);

    putchar('\n');
}

void Logger::error(const char* format, ...) {
    va_list args;

    printf("\e[0;31mError: \e[0m");

    va_start(args, format);

    vprintf(format, args);

    va_end(args);

    putchar('\n');
}

void Logger::print(const char* format, ...) {
    va_list args;

    va_start(args, format);

    vprintf(format, args);

    va_end(args);

    putchar('\n');
}

} // namespace age
