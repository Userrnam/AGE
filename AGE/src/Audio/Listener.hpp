#pragma once

#include "Math.hpp"

namespace age {

struct Orientation {
    Vector3f at = {};
    Vector3f up = {0,1,0};
};

class Listener {
    static Vector3f m_position;
    static Vector3f m_velocity;
    static Orientation m_orientation;
public:
    static void setVelocity(const Vector3f& velocity = {});
    static void setPosition(const Vector3f& position = {});
    static void setOrientation(const Orientation& orientation);

    static void setVelocity(float vx, float vy, float vz);
    static void setPosition(float x, float y, float z);

    static Vector3f getVelocity();
    static Vector3f getPosition();
    static Orientation getOrientation();
};

} // namespace age
