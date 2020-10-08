#include "OpenAL.hpp"

#include "Listener.hpp"


namespace age {

Vector3f Listener::m_position = {};
Vector3f Listener::m_velocity = {};
Orientation Listener::m_orientation = Orientation();

void Listener::setVelocity(const Vector3f& velocity) {
    alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
    ERROR_CHECK("Velocity");
    m_velocity = velocity;
}

void Listener::setPosition(const Vector3f& position) {
    alListener3f(AL_POSITION, position.x, position.y, position.z);
    ERROR_CHECK("Position");
    m_position = position;
}

void Listener::setOrientation(const Orientation& orientation) {
    alListenerfv(AL_ORIENTATION, (ALfloat*)&orientation);
    ERROR_CHECK("Orientation");
    m_orientation = orientation;
}

void Listener::setVelocity(float vx, float vy, float vz) {
    Listener::setVelocity({vx, vy, vz});
}

void Listener::setPosition(float x, float y, float z) {
    Listener::setPosition({x, y, z});
}

Vector3f Listener::getVelocity() {
    return m_velocity;
}

Vector3f Listener::getPosition() {
    return m_position;
}

Orientation Listener::getOrientation() {
    return m_orientation;
}

} // namespace age
