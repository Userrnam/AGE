#include "Core.hpp"
#include <AL/al.h>

namespace age::audio {

void Core::init() {
    m_device = alcOpenDevice(nullptr);
    if (!m_device) {
        throw std::runtime_error("Failed to open audio device");
    }

    m_context = alcCreateContext(m_device, nullptr);
    if (!m_context) {
        throw std::runtime_error("Failed to create context");
    }

    if (!alcMakeContextCurrent(m_context)) {
        throw std::runtime_error("Failed to make context current");
    }    
}

void Core::destroy() {
    alcDestroyContext(m_context);
    alcCloseDevice(m_device);
}

} // namespace age::audio
