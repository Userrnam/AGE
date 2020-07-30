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
    int32_t* m_counter;
public:
    T data;

#ifndef NDEBUG
    struct {
        char* name = nullptr;
        bool created = false;
    } debug;
#endif

    inline Shared<T> copy() {
#ifndef NDEBUG
        if (!debug.created) {
            print_message(*this, " Shared has not been created before copy");
        }
#endif

        (*m_counter)++;
        return *this;
    }

    inline void destroy() {
        (*m_counter)--;
        if (!(*m_counter)) {
            data.destroy();
            free(m_counter);
        }
#ifndef NDEBUG
        else if ((*m_counter) < 0) {
            print_message(*this, " Shared has been destroyed multiple times");
        }
#endif
    }

    inline void create(T _data) {
        m_counter = (int32_t*)malloc(sizeof(int32_t));
        (*m_counter) = 1;
        data = _data;
#ifndef NDEBUG
        debug.created = true;
#endif
    }
};

} // namespace age
