#pragma once

#include <glm/glm.hpp>

#include "Buffer.hpp"
#include "Descriptor.hpp"
#include "Transformable.hpp"

namespace age {

class Viewport;

struct CameraUniform {
    glm::mat4 m_projection = glm::mat4(1.0f);
    glm::vec4 m_time = glm::vec4(0.0f);
};

class Camera : public Transformable {
    Descriptor m_descriptor;
    Buffer m_buffer;
    CameraUniform m_uniform;
    bool m_projectionChanged = false;
    friend class Scene;
public:
    void create();
    void destroy();

    Descriptor getDescriptor() const { return m_descriptor; }

    void setOrthoganalProjection(const Viewport& vieport, float zNear = 0.0f, float zFar = 1.0f);
    void setPerspectiveProjection(const Viewport& viewport, float angle = 3.14/4., float zNear=0.1f, float zFar=10.0f);

    void upload();
};

} // namespace age
