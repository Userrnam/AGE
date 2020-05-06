#pragma once

#include <glm/glm.hpp>

namespace age {

class Transformable {
    glm::vec2 m_scale = glm::vec2(1.0f);
    glm::vec2 m_position = {};
    glm::vec2 m_origin = {};
    float m_rotation = 0;
protected:
    glm::mat4 getTransform();
public:
    glm::vec2 getScale() const { return m_scale; }
    glm::vec2 getPosition() const { return m_position; }
    glm::vec2 getOrigin() const { return m_origin; }
    float getRotation() const { return m_rotation; }

    void setScale(const glm::vec2& scale) { m_scale = scale; }
    void setPosition(const glm::vec2& position) { m_position = position; }
    void setOrigin(const glm::vec2& origin) { m_origin = origin; }
    void setRotation(float rotation) { m_rotation = rotation; }

    void move(const glm::vec2& direction) { m_position += direction; }
    void rotate(float angle) { m_rotation += angle; }

    void setScale(float scaleX, float scaleY) { m_scale.x = scaleX; m_scale.y = scaleY; }
    void setPosition(float x, float y) { m_position.x = x; m_position.y = y; }
    void setOrigin(float x, float y) { m_origin.x = x; m_origin.y = y; }
    void move(float x, float y) { m_position.x += x; m_position.y += y; }
};

} // namespace age
