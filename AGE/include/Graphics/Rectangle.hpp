#pragma once

#include <glm/glm.hpp>

#include "Object.hpp"
#include "Core/Buffer.hpp"
#include "Transformable.hpp"
#include "View.hpp"

namespace age {

/*

Rectangles origin must be in range 0-1

*/

struct RectangleUniform {
    glm::mat4 transform;
    glm::vec4 color;
};

class Rectangle : public Object, public Transformable {
    core::Buffer m_uboBuffer;
    glm::vec4 m_color = {};
    bool m_isOwner;
public:
    void create(const Rectangle& sample);
    void create(const View& view);
    void destroy();

    glm::vec4 getColor() const { return m_color; }
    void setColor(const glm::vec4& color);

    void setUniform(const RectangleUniform& uniform);
    void upload();
};

} // namespace age
