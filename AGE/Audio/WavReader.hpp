#pragma once

#include <vector>

#include "OpenAL.hpp"

namespace age {

class WavReader {
    std::vector<char> m_buf;

    ALenum m_format;
    ALsizei m_size;
    ALsizei m_freq;

public:
    inline const ALvoid* getData() const { return m_buf.data(); }
    inline ALenum getFormat() const { return m_format; }
    inline ALsizei getSize() const { return m_size; }
    inline ALsizei getFreq() const { return m_freq; }

    void read(const std::string& path);
};

} // namespace age
