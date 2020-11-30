#include "Transformable.hpp"

#include "PositionManager.hpp"
#include "../Scene/Entity.hpp"
#include "../Scene/Scene.hpp"

namespace age {

void Transformable::create(const Entity& e, Vector2f size) {
    m_entity = e;
    m_size = size;

    getTransform();
}

void Transformable::destroy() {
    m_entity.getScene()->getPositionManager()->remove(m_entity.getEntityId());
}

glm::mat4 Transformable::getTransform() {
    auto transform = UnmanagedTransformable::getTransform();

    // get transformed bounding box
    glm::vec4 bottomLeft = { 0, 0, -1, 1 };
    glm::vec4 bottmRight = { m_size.x, 0, -1, 1 };
    glm::vec4 topLeft    = { 0, m_size.y, -1, 1 };
    glm::vec4 topRight   = { m_size.x, m_size.y, -1, 1 };

    glm::vec4 newBottomLeft  = transform * bottomLeft;
    glm::vec4 newBottomRight = transform * bottmRight;
    glm::vec4 newTopLeft     = transform * topLeft;
    glm::vec4 newTopRight    = transform * topRight;

    float xs[4] = { newBottomLeft.x, newBottomRight.x, newTopLeft.x, newTopRight.x };
    float ys[4] = { newBottomLeft.y, newBottomRight.y, newTopLeft.y, newTopRight.y };
    float minX = newBottomLeft.x;
    float maxX = newBottomLeft.x;
    float minY = newBottomLeft.y;
    float maxY = newBottomLeft.y;

    for (int i = 0; i < 4; ++i) {
        if (xs[i] < minX) {
            minX = xs[i];
        } else if (xs[i] > maxX) {
            maxX = xs[i];
        }

        if (ys[i] < minY) {
            minY = ys[i];
        } else if (ys[i] > maxY) {
            maxY = ys[i];
        }
    }

    Positionable p;
    p.pos = { minX - getOrigin().x * m_size.x, minY - getOrigin().y * m_size.y };
    p.size = { maxX - minX, maxY - minY };

    m_entity.getScene()->getPositionManager()->update(m_entity.getEntityId(), p);

    return transform;
}

} // namespace age
