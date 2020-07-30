#pragma once

#include <glm/glm.hpp>

#include "Buffer.hpp"

namespace age {

class Transformable {
    glm::vec2 m_scale = glm::vec2(1.0f);
    glm::vec2 m_position = {};
    glm::vec2 m_origin = {};
    float m_rotation = 0;
    bool m_needUpdate = true;
protected:
    glm::mat4 m_transform;
public:
    glm::mat4 getTransform();

    glm::vec2 getScale() const { return m_scale; }
    glm::vec2 getPosition() const { return m_position; }
    glm::vec2 getOrigin() const { return m_origin; }
    float getRotation() const { return m_rotation; }

    void setScale(const glm::vec2& scale);
    void setPosition(const glm::vec2& position);
    void setOrigin(const glm::vec2& origin);
    void setRotation(float rotation);

    void move(const glm::vec2& direction);
    void rotate(float angle);

    void setScale(float scaleX, float scaleY);
    void setPosition(float x, float y);
    void setOrigin(float x, float y);
    void move(float x, float y);
};

} // namespace age
