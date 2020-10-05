#include <glm/gtc/matrix_transform.hpp>

#include "Transformable.hpp"


namespace age {

glm::mat4 Transformable::getTransform() {
    // FIXME:
    auto originMat = glm::translate(glm::mat4(1.0f), glm::vec3(-m_origin.x, -m_origin.y, 0));
    auto translateMat = glm::translate(glm::mat4(1.0f), glm::vec3(m_position.x, m_position.y, 0));
    auto rotateMat = glm::rotate(glm::mat4(1.0f), m_rotation, glm::vec3(0, 0, 1));
    auto scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(m_scale.x, m_scale.y, 1));
    auto transform = translateMat * rotateMat * scaleMat * originMat;

    return transform;
}

Transformable& Transformable::setScale(const glm::vec2& scale) {
    m_scale = scale;
    return *this;
}

Transformable& Transformable::setPosition(const glm::vec2& position) {
    m_position = position;
    return *this;
}

Transformable& Transformable::setOrigin(const glm::vec2& origin) {
    m_origin = origin;
    return *this;
}

Transformable& Transformable::setRotation(float rotation) {
    m_rotation = rotation;
    return *this;
}

Transformable& Transformable::move(const glm::vec2& direction) {
    m_position += direction;
    return *this;
}

Transformable& Transformable::rotate(float angle) {
    m_rotation += angle;
    return *this;
}

Transformable& Transformable::setScale(float scaleX, float scaleY) {
    m_scale.x = scaleX;
    m_scale.y = scaleY;
    return *this;
}

Transformable& Transformable::setPosition(float x, float y) {
    m_position.x = x;
    m_position.y = y;
    return *this;
}

Transformable& Transformable::setOrigin(float x, float y) {
    m_origin.x = x;
    m_origin.y = y;
    return *this;
}

Transformable& Transformable::move(float x, float y) {
    m_position.x += x;
    m_position.y += y;
    return *this;
}

} // namespace age
