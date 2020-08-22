#pragma once

#include "entt.hpp"

namespace age {

class Scene;
class Entity {
    entt::entity m_entityId;
    entt::registry* m_registry;

    friend class Scene;
    friend class ScriptComponent;
public:
    // Warning: all entitiy's components must be destroyed before calling this
    void destroy() {
        m_registry->destroy(m_entityId);
    }

    template<typename T>
    inline T& addComponent() {
        T component;
        m_registry->emplace<T>(m_entityId, component);
        return m_registry->get<T>(m_entityId);
    }

    template<typename T>
    inline T& addComponent(T component) {
        m_registry->emplace<T>(m_entityId, component);
        return m_registry->get<T>(m_entityId);
    }

    template<typename T>
    inline T& getComponent() {
        return m_registry->get<T>(m_entityId);
    }
};

} // namespace age
