#pragma once

#include <vector>
#include <stdint.h>

namespace age {

class Arena {
    size_t m_blockSize;

    std::vector<uint8_t*> m_data;
    size_t m_blockIndex = 0;
    size_t m_position = 0;
public:
    void init(size_t blockSize = 1024 * 1024);
    void destroy();
    void* allocate(size_t size);
    void flush();
};

} // namespace age
