#include <vector>
#include <stdlib.h>

#include "Arena.hpp"

namespace age {

std::vector<uint8_t*> Arena::m_data;
size_t Arena::m_blockIndex = 0;
size_t Arena::m_position = 0;
size_t Arena::m_blockSize = 0;

void Arena::init(size_t blockSize) {
    m_blockSize = blockSize;
    uint8_t* mem = (uint8_t*)malloc(m_blockSize);
    m_data.push_back(mem);
}

void Arena::destroy() {
    for (auto mem : m_data) {
        free(mem);
    }
}

void* Arena::allocate(size_t size) {
    // enough space in this block
    if (m_position + size < m_blockSize) {
        uint8_t* ret = m_data[m_blockIndex] + m_position;
        m_position += size;
        return ret;
    }

    m_blockIndex++;
    m_position = 0;

    // not enough blocks
    if (m_data.size() > m_blockIndex) {
        uint8_t* mem = (uint8_t*)malloc(m_blockSize);
        m_data.push_back(mem);
    }

    void* ret = m_data[m_blockIndex] + m_position;
    m_position += size;
    return ret;
}

void Arena::flush() {
    m_position = 0;
    m_blockIndex = 0;
}

} // namespace age
