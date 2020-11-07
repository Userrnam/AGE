#pragma once

#include <glm/glm.hpp>

namespace age {

class Camera {
    glm::mat4 m_projection;
    // friend class Scene;
public:
    void setOrthoganalProjection(float width, float height, float zNear = 0.0f, float zFar = 1.0f);
    void setPerspectiveProjection(float width, float height, float angle = 3.14/4., float zNear=0.1f, float zFar=10.0f);

    inline glm::mat4 getProjection() const {
        return m_projection;
    }
};

} // namespace age
