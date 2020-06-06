#pragma once

#include "OpenAL.hpp"

namespace age::audio {

class Core {
    ALCdevice* m_device;
    ALCcontext* m_context;
public:
// TODO: add ability to choose device
    void init();
    void destroy();

    ALCdevice* const getDevice() const { return m_device; }
    ALCcontext* const getContext() const { return m_context; }
};

} // namespace age::audio
