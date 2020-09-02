#pragma once

#include <glm/glm.hpp>

#include "MemoryHolders/Buffer.hpp"

namespace age {

class Transformable {
    glm::vec2 m_scale = glm::vec2(1.0f);
    glm::vec2 m_position = {};
    glm::vec2 m_origin = {};
    float m_rotation = 0;
protected:
    glm::mat4 m_transform;
public:
    glm::mat4 getTransform();

    inline glm::vec2 getScale() const { return m_scale; }
    inline glm::vec2 getPosition() const { return m_position; }
    inline glm::vec2 getOrigin() const { return m_origin; }
    inline float getRotation() const { return m_rotation; }

    Transformable& setScale(const glm::vec2& scale);
    Transformable& setPosition(const glm::vec2& position);
    Transformable& setOrigin(const glm::vec2& origin);
    Transformable& setRotation(float rotation);

    Transformable& move(const glm::vec2& direction);
    Transformable& rotate(float angle);

    Transformable& setScale(float scaleX, float scaleY);
    Transformable& setPosition(float x, float y);
    Transformable& setOrigin(float x, float y);

    Transformable& move(float x, float y);
};

} // namespace age
