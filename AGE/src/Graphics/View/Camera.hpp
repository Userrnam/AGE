#pragma once

#include <glm/glm.hpp>

namespace age {

class Viewport;

class Camera {
    glm::mat4 m_projection;
    // friend class Scene;
public:
    void setOrthoganalProjection(const Viewport& vieport, float zNear = 0.0f, float zFar = 1.0f);
    void setPerspectiveProjection(const Viewport& viewport, float angle = 3.14/4., float zNear=0.1f, float zFar=10.0f);

    inline glm::mat4 getProjection() const {
        return m_projection;
    }
};

} // namespace age
