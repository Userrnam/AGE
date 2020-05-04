#pragma once

#include <glm/glm.hpp>

#include "Object.hpp"
#include "Core/Buffer.hpp"

namespace age {

struct RectangleUniform {
    glm::mat4 transform;
    glm::vec4 color;
};

class Rectangle : public Object {
    core::Buffer m_uboBuffer;
    glm::vec4 m_color = {};
    glm::vec2 m_size = {};
    glm::vec2 m_pos = {};
    glm::vec2 m_origin = {};
    float m_rotation = 0;
public:
    void create(bool cameraAccess = true);
    void destroy();

    glm::vec2 getSize() const { return m_size; }
    glm::vec2 getPosition() const { return m_pos; }
    glm::vec4 getColor() const { return m_color; }

    void setColor(const glm::vec4& color);
    void setSize(const glm::vec2& size);
    void setPosition(const glm::vec2& pos);
    // void setRotation(const float rotation);
    void setOrigin(const glm::vec2& origin);

    void setUniform(const RectangleUniform& uniform);
    void upload();
};

} // namespace age
