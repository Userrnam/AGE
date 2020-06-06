#include "OpenAL.hpp"

#include "Listener.hpp"


namespace age {

glm::vec3 Listener::m_position = {};
glm::vec3 Listener::m_velocity = {};
Orientation Listener::m_orientation = Orientation();

void Listener::setVelocity(const glm::vec3& velocity) {
    alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
    ERROR_CHECK("Velocity")
    m_velocity = velocity;
}

void Listener::setPosition(const glm::vec3& position) {
    alListener3f(AL_POSITION, position.x, position.y, position.z);
    ERROR_CHECK("Position")
    m_position = position;
}

void Listener::setOrientation(const Orientation& orientation) {
    alListenerfv(AL_ORIENTATION, (ALfloat*)&orientation);
    ERROR_CHECK("Orientation")
    m_orientation = orientation;
}

void Listener::setVelocity(float vx, float vy, float vz) {
    Listener::setVelocity({vx, vy, vz});
}

void Listener::setPosition(float x, float y, float z) {
    Listener::setPosition({x, y, z});
}

glm::vec3 Listener::getVelocity() {
    return m_velocity;
}

glm::vec3 Listener::getPosition() {
    return m_position;
}

Orientation Listener::getOrientation() {
    return m_orientation;
}

} // namespace age
