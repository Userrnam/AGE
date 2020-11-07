#pragma once

#include <string>

namespace age {

class Sound {
    unsigned int m_alBuffer;

    friend class SoundSource;
public:
    void create();
    void destroy();

    void load(const std::string& file);
};

} // namespace age
