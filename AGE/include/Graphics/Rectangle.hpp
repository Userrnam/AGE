#pragma once

#include <glm/glm.hpp>

#include "Object.hpp"
#include "Core/Buffer.hpp"
#include "Transformable.hpp"
#include "View.hpp"
#include "Texture.hpp"

namespace age {

struct RectangleUniform {
    glm::mat4 transform;
    glm::vec4 color;
};

class Rectangle : public Object, public Transformable {
    core::Buffer m_uboBuffer;
    glm::vec4 m_color = {};
    bool m_isOwner;

    void preCreate(const View& view, ObjectCreateInfo& objectCreateInfo);
public:
    void create(const Rectangle& sample);
    void create(const Rectangle& sample, const Texture& texture);
    void create(const View& view);
    void create(const View& view, const Texture& texture);
    void destroy();

    glm::vec4 getColor() const { return m_color; }
    void setColor(const glm::vec4& color);
    void setColor(float r, float g, float b, float a) { m_color.r = r; m_color.g = g; m_color.b = b; m_color.a = a; }

    void setUniform(const RectangleUniform& uniform);
    void upload();
};

} // namespace age
