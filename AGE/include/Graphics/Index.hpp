#pragma once

#include <stdint.h>

namespace age {

class Index16 {
    uint16_t m_i;
public:
    Index16(uint16_t i) : m_i(i) {}
    static uint32_t getIndexType();
};

class Index32 {
    uint32_t m_i;
public:
    Index32(uint32_t i) : m_i(i) {}
    static uint32_t getIndexType();
};


} // namespace age
