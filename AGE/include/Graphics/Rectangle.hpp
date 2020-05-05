#pragma once

#include <glm/glm.hpp>

#include "Object.hpp"
#include "Core/Buffer.hpp"
#include "Transformable.hpp"

namespace age {

struct RectangleUniform {
    glm::mat4 transform;
    glm::vec4 color;
};

class Rectangle : public Object, public Transformable {
    core::Buffer m_uboBuffer;
    glm::vec4 m_color = {};
public:
    void create(bool cameraAccess = true);
    void destroy();

    glm::vec4 getColor() const { return m_color; }
    void setColor(const glm::vec4& color);

    void setUniform(const RectangleUniform& uniform);
    void upload();
};

} // namespace age
