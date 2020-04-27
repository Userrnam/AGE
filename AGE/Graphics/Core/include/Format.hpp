#pragma once

namespace age {

enum FormatCounter {
    COUNT_4,
    COUNT_8,
    COUNT_32,
    COUNT_64
};

// use 2 bits per r, g, b, a
constexpr
uint32_t makeFormat(FormatCounter r, FormatCounter g, FormatCounter b,
FormatCounter a, bool sign, bool norm) {
    return r | (g << 2) | (b << 4) | (a << 8) | (sign << 9) | (norm << 10);
}
    
} // namespace age
