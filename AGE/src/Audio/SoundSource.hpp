#pragma once

#include <glm/glm.hpp>

#include "Sound.hpp"

namespace age {

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
    void setPosition(const glm::vec3& position);
    void setVelocity(const glm::vec3& velocity);
    void setPitch(float pitch);
    void setGain(float gain);
    void setLooping(bool b);

    void setPosition(float x, float y, float z) {
        setPosition({x, y, z});
    }

    void setVelocity(float vx, float vy, float vz) {
        setVelocity({vx, vy, vz});
    }

    int getState();
};

} // namespace age
