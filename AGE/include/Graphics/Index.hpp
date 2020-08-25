#pragma once

#include <stdint.h>

namespace age {

class Index16 {
    uint16_t m_i;
public:
    Index16(uint16_t i) : m_i(i) {}
};

class Index32 {
    uint32_t m_i;
public:
    Index32(uint32_t i) : m_i(i) {}
};


} // namespace age
