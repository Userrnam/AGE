#pragma once

#include "../Graphics/Transformable.hpp"
#include "../Graphics/ObjectCreation/Components/TransformComponent.hpp"

namespace age {

class TransformResolveStructure : public IResolveStructure {
    Transform* m_transform;
    Transformable* m_transformable;
public:
    TransformResolveStructure(Transform* transform, Transformable* transformable)
        : m_transform(transform), m_transformable(transformable) {}

    virtual void resolve () override {
        m_transform->set(m_transformable->getTransform());
    }
};

} // namespace age
