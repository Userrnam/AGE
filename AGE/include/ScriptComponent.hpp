#pragma once

#include "Event.hpp"
#include "Entity.hpp"

namespace age {

struct ScriptComponent : public Entity {
    void create(Entity e) {
        m_entityId = e.m_entityId;
        m_registry = e.m_registry;

        onCreate();
    }

    virtual void onCreate() {}
    virtual void onDestroy() {}
    virtual void onEvent(Event event) {}
    virtual void onUpdate(float elapsedTime) {}
};

} // namespace age
