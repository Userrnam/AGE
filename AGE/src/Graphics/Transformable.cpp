#include <iostream>

#include "Transformable.hpp"

#include "PositionManager.hpp"
#include "../Scene/Entity.hpp"

namespace age {

void Transformable::create(const Entity& e, Vector2f size) {
    m_entity = e;
    m_size = size;
}

void Transformable::destroy() {
    m_entity.getScene()->getPositionManager()->remove(m_entity.getEntityId());
}

glm::mat4 Transformable::getTransform() {
    auto transform = UnmanagedTransformable::getTransform();

    // get transformed bounding box
    glm::vec4 bottomLeft = { 0, 0, -1, 1 };
    glm::vec4 topRight = { m_size.x, m_size.y, -1, 1 };

    glm::vec4 newBottomLeft = transform * bottomLeft;
    glm::vec4 newTopRight = transform * topRight;

    Positionable p;
    p.pos = { newBottomLeft.x, newBottomLeft.y };
    p.size = { newTopRight.x - newBottomLeft.x, newTopRight.y - newBottomLeft.y };

    m_entity.getScene()->getPositionManager()->update(m_entity.getEntityId(), p);

    return transform;
}

} // namespace age
