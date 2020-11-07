#pragma once

#include "../External/entt.hpp"
#include "PositionManager.hpp"
#include "../Scene/Entity.hpp"
#include "UnmanagedTransformable.hpp"

namespace age {

class Transformable : public UnmanagedTransformable {
    Entity m_entity;
    Vector2f m_size;
public:
    void create(const Entity& e, Vector2f size = {1, 1});
    void setSize(const Vector2f& size) { m_size = size; }
    void destroy();

    glm::mat4 getTransform();
    void informAboutUpdate() { getTransform(); }
};

} // namespace age
