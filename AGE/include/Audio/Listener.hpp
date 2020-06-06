#pragma once

#include <glm/glm.hpp>

namespace age {

struct Orientation {
    glm::vec3 at = {};
    glm::vec3 up = {0,1,0};
};

class Listener {
    static glm::vec3 m_position;
    static glm::vec3 m_velocity;
    static Orientation m_orientation;
public:
    static void setVelocity(const glm::vec3& velocity = {});
    static void setPosition(const glm::vec3& position = {});
    static void setOrientation(const Orientation& orientation);

    static void setVelocity(float vx, float vy, float vz);
    static void setPosition(float x, float y, float z);

    static glm::vec3 getVelocity();
    static glm::vec3 getPosition();
    static Orientation getOrientation();
};

} // namespace age
