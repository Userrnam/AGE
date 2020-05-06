#include <glm/gtc/matrix_transform.hpp>

#include "Transformable.hpp"


namespace age {

glm::mat4 Transformable::getTransform() {
    if (m_needUpdate) {
        auto originMat = glm::translate(glm::mat4(1.0f), glm::vec3(-m_origin.x, -m_origin.y, 0));
        auto translateMat = glm::translate(glm::mat4(1.0f), glm::vec3(m_position.x, m_position.y, 0));
        auto rotateMat = glm::rotate(glm::mat4(1.0f), m_rotation, glm::vec3(0, 0, 1));
        auto scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(m_scale.x, m_scale.y, 1));
        m_transform = translateMat * rotateMat * scaleMat * originMat;

        m_needUpdate = false;
    }

    return m_transform;
}

void Transformable::setScale(const glm::vec2& scale) {
    m_scale = scale;
    m_needUpdate = true;
}

void Transformable::setPosition(const glm::vec2& position) {
    m_position = position;
    m_needUpdate = true;
}

void Transformable::setOrigin(const glm::vec2& origin) {
    m_origin = origin;
    m_needUpdate = true;
}

void Transformable::setRotation(float rotation) {
    m_rotation = rotation;
    m_needUpdate = true;
}

void Transformable::move(const glm::vec2& direction) {
    m_position += direction;
    m_needUpdate = true;
}

void Transformable::rotate(float angle) {
    m_rotation += angle;
    m_needUpdate = true;
}

void Transformable::setScale(float scaleX, float scaleY) {
    m_scale.x = scaleX;
    m_scale.y = scaleY;
    m_needUpdate = true;
}

void Transformable::setPosition(float x, float y) {
    m_position.x = x;
    m_position.y = y;
    m_needUpdate = true;
}

void Transformable::setOrigin(float x, float y) {
    m_origin.x = x;
    m_origin.y = y;
    m_needUpdate = true;
}

void Transformable::move(float x, float y) {
    m_position.x += x;
    m_position.y += y;
    m_needUpdate = true;
}

} // namespace age
