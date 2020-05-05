#pragma once

#include <glm/glm.hpp>

namespace age {

class Transformable {
    glm::vec2 m_scale = {};
    glm::vec2 m_pos = {};
    glm::vec2 m_origin = {};
    float m_rotation = 0;
protected:
    glm::mat4 getTransform();
public:
    glm::vec2 getScale() const { return m_scale; }
    glm::vec2 getPosition() const { return m_pos; }
    glm::vec2 getOrigin() const { return m_origin; }
    float getRotation() const { return m_rotation; }

    void setScale(const glm::vec2& scale) { m_scale = scale; }
    void setPosition(const glm::vec2& position) { m_pos = position; }
    void setOrigin(const glm::vec2& origin) { m_origin = origin; }
    void setRotation(const float rotation) { m_rotation = rotation; }
};
    
} // namespace age

