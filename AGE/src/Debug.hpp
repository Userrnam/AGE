#pragma once

#ifndef NDEBUG
#include <iostream>

#define ASSERT(condition, message) \
    if (!condition) { \
        std::cerr << message << std::endl; \
        std::terminate(); \
    }

#define set_name(namable, name) (namable).debug.name = name

#define print_message(namable, message) \
    do { \
        if ((namable).debug.name) { \
            std::cout << (namable).debug.name << message << std::endl; \
        } else { \
            std::cout << message << std::endl; \
        } \
    } while (0)

#define print_name(namable) print_message(namable, " at " __FILE__ "; " __LINE__)

#else
#define ASSERT(condition, message)
#define print_message(namable, message)
#define set_name(namable, name)
#define print_name(namable)
#endif
