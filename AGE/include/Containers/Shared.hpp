#pragma once

#include <stdint.h>

#include "Debug.hpp"

namespace age {

/*
    Same as std::shared_ptr, but is not a pointer and doesnt have constructor / desctructor
    T must have destroy method
*/
template<typename T>
class Shared {
    int32_t m_counter;
public:
    T data;

#ifndef NDEBUG
    struct {
        char* name = nullptr;
        bool created = false;
        int counter = 0;
    } debug;
#endif

    inline T copy() {
#ifndef NDEBUG
        if (!debug.created) {
            print_message(*this, " Shared has not been created before copy");
        }
#endif

        m_counter++;
        return *this;
    }

    inline void destroy() {
        m_counter--;
        if (!m_counter) {
            data.destroy();
        }
#ifndef NDEBUG
        else if (m_counter < 0) {
            print_message(*this, " Shared has been destroyed multiple times");
        }
#endif
    }

    inline void create(T _data) {
        m_counter = 1;
        data = _data;
#ifndef NDEBUG
        debug.created = true;
#endif
    }

#ifndef NDEBUG
    Shared() {
        debug.counter++;
    }

    Shared(const Shared& s) {
        *this = s;
        debug.counter++;
    }

    ~Shared() {
        debug.counter--;
        if (debug.counter == 0 && m_counter > 0) {
            print_message(*this, " Shared has not been destroyed");
        }
    }
#endif
};

} // namespace age
