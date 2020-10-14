#pragma once

#include "Math.hpp"

#include "Sound.hpp"

namespace age {

enum class SoundSourceState {
    INITIAL,
    PLAYING,
    PAUSED,
    STOPPED,
    ERROR
};

class SoundSource {
    unsigned int m_alSource = 0;
public:
    void create();
    void destroy();

    void play();
    void stop();
    void pause();
    void rewind();

    void setSound(const Sound& sound);
    void setPosition(const Vector3f& position);
    void setVelocity(const Vector3f& velocity);
    void setPitch(float pitch);
    void setGain(float gain);
    void setLooping(bool b);

    void setPosition(float x, float y, float z) {
        setPosition({x, y, z});
    }

    void setVelocity(float vx, float vy, float vz) {
        setVelocity({vx, vy, vz});
    }

    SoundSourceState getState();
};

} // namespace age
