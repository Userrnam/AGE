#include <glm/gtc/matrix_transform.hpp>

#include "UnmanagedTransformable.hpp"

namespace age {

glm::mat4 UnmanagedTransformable::getTransform() {
    // FIXME:
    auto originMat = glm::translate(glm::mat4(1.0f), { -m_origin.x, -m_origin.y, 0});
    auto translateMat = glm::translate(glm::mat4(1.0f), { m_position.x, m_position.y, 0});
    auto rotateMat = glm::rotate(glm::mat4(1.0f), m_rotation, {0, 0, 1});
    auto scaleMat = glm::scale(glm::mat4(1.0f), {m_scale.x, m_scale.y, 1});
    auto transform = translateMat * rotateMat * scaleMat * originMat;

    return transform;
}

void UnmanagedTransformable::setScale(const Vector2f& scale) {
    m_scale = scale;
}

void UnmanagedTransformable::setPosition(const Vector2f& position) {
    m_position = position;
}

void UnmanagedTransformable::setOrigin(const Vector2f& origin) {
    m_origin = origin;
}

void UnmanagedTransformable::setRotation(float rotation) {
    m_rotation = rotation;
}

void UnmanagedTransformable::scale(const Vector2f& scale) {
    m_scale *= scale;
}

void UnmanagedTransformable::move(const Vector2f& direction) {
    m_position += direction;
}

void UnmanagedTransformable::rotate(float angle) {
    m_rotation += angle;
}

void UnmanagedTransformable::setScale(float scaleX, float scaleY) {
    m_scale.x = scaleX;
    m_scale.y = scaleY;
}

void UnmanagedTransformable::setPosition(float x, float y) {
    m_position.x = x;
    m_position.y = y;
}

void UnmanagedTransformable::setOrigin(float x, float y) {
    m_origin.x = x;
    m_origin.y = y;
}

void UnmanagedTransformable::scale(float xs, float ys) {
    m_scale.x *= xs;
    m_scale.y *= ys;
}

void UnmanagedTransformable::move(float x, float y) {
    m_position.x += x;
    m_position.y += y;
}

} // namespace age
