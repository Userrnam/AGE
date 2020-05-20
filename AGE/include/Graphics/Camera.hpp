#pragma once

#include <glm/glm.hpp>

#include "Buffer.hpp"
#include "Descriptor.hpp"
#include "Transformable.hpp"

namespace age {

class View;

class Camera : public Transformable {
    Descriptor m_descriptor;
    Buffer m_buffer;
    glm::mat4 m_projection = glm::mat4(1.0f);

    // FIXME
    friend class Rectangle;
    friend class RectangleFactory;
    friend class TexturedRectangleFactory;
public:
    Camera();
    ~Camera();

    Descriptor getDescriptor() const { return m_descriptor; }

    void setOrthoganalProjection(View& view, float zNear = 0.0f, float zFar = 1.0f);
    void setPerspectiveProjection(View& view, float angle = 3.14/4., float zNear=0.1f, float zFar=10.0f);

    void upload();
};

} // namespace age
