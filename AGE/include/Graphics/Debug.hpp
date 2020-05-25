#pragma once

#ifdef DEBUG

#include <iostream>

#define ASSERT(condition, message) \
    if (!condition) { \
        std::cerr << message << std::endl; \
        std::terminate(); \
    }
#else

#define ASSERT(condition, message)

#endif
