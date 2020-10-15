#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
Vector/DynamicBuffer time
append: 9
[]:     0.8
insert: 4
*/

namespace age {

class DynamicBuffer {
    void* m_data;
    uint32_t m_index = 0;
    uint32_t m_count = 0;
    uint32_t m_allocSize = 0;

public:
    template<typename T>
    inline void create(uint32_t count = 8) {
        m_count = count;
        m_allocSize = m_count * sizeof(T);
        m_data = malloc(m_allocSize);
    }

    DynamicBuffer copy() {
        DynamicBuffer buffer;
        buffer.m_index = m_index;
        buffer.m_count = m_count;
        buffer.m_allocSize = m_allocSize;
        m_data = malloc(m_allocSize);
        memcpy(buffer.m_data, m_data, m_allocSize);

        return buffer;
    }

    inline void destroy() {
        free(m_data);
    }

    // unlike usual vector no bound check
    template<typename T>
    inline T& at(uint32_t pos) {
        return ((T*)m_data)[pos];
    }

    template<typename T = void>
    inline T* data() {
        return (T*)m_data;
    }

    inline bool empty() {
        return m_index == 0;
    }

    inline uint32_t count() {
        return m_index;
    }

    inline uint32_t size() {
        return m_allocSize;
    }

    template<typename T>
    inline void reserve(uint32_t count) {
        m_count = count;
        m_allocSize = m_count * sizeof(T);
        m_data = realloc(m_data, m_allocSize);
    }

    template<typename T>
    inline void resize(uint32_t count) {
        m_index = count;
        m_count = count;
        m_allocSize = m_count * sizeof(T);
        m_data = realloc(m_data, m_allocSize);
    }

    inline uint32_t capacity() {
        return m_count;
    }

    template<typename T>
    inline void shrink() {
        m_count = m_index;
        m_allocSize = m_count * sizeof(T);
        m_data = realloc(m_data, m_allocSize);
    }

    inline void clear() {
        m_index = 0;
    }

    template<typename T>
    void insert(uint32_t pos, const T& elem) {
        // !! unsafe should be checking allocSize
        if (m_index == m_count) {
            m_count *= 2;
            m_allocSize *= 2;
            m_data = realloc(m_data, m_allocSize);
        }
        auto data = (T*)m_data;
        memmove(data + 1, data, m_count - pos);
        m_index++;
    }

    template<typename T>
    void append(const T& elem) {
        if (m_index == m_count) {
            m_count *= 2;
            m_allocSize *= 2;
            m_data = realloc(m_data, m_allocSize);
        }

        auto data = (T*)m_data;
        data[m_index] = elem;
        m_index++;
    }

    inline void pop() {
        m_index--;
    }
};

} // namespace age
