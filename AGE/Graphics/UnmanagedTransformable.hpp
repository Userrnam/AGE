#pragma once

#include <glm/glm.hpp>

#include "../Math/Vector.hpp"

namespace age {

class UnmanagedTransformable {
    Vector2f m_scale = Vector2f(1.0f);
    Vector2f m_position = {};
    Vector2f m_origin = {};
    float m_rotation = 0;
public:
    glm::mat4 getTransform();

    inline Vector2f getScale() const { return m_scale; }
    inline Vector2f getPosition() const { return m_position; }
    inline Vector2f getOrigin() const { return m_origin; }
    inline float getRotation() const { return m_rotation; }

    inline Vector2f* getScalePointer() { return &m_scale; }
    inline Vector2f* getPositionPointer() { return &m_position; }
    inline Vector2f* getOriginPointer() { return &m_origin; }
    inline float* getRotationPointer() { return &m_rotation; }

    void setScale(const Vector2f& scale);
    void setPosition(const Vector2f& position);
    void setOrigin(const Vector2f& origin);
    void setRotation(float rotation);

    void scale(const Vector2f& scale);
    void move(const Vector2f& direction);
    void rotate(float angle);

    void setScale(float scaleX, float scaleY);
    void setPosition(float x, float y);
    void setOrigin(float x, float y);

    void scale(float scaleX, float scaleY);
    void move(float x, float y);
};

} // namespace age
