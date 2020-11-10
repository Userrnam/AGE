#pragma once

#include <vector>
#include <stdint.h>

namespace age {

class Arena {
    static size_t m_blockSize;

    static std::vector<uint8_t*> m_data;
    static size_t m_blockIndex;
    static size_t m_position;
public:
    static void init(size_t blockSize);
    static void destroy();

    static void* allocate(size_t size);
    static void flush();
};

} // namespace age
