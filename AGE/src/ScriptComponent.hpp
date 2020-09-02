#pragma once

#include "Events/Event.hpp"
#include "Scene/Entity.hpp"

namespace age {

struct ScriptComponent : public Entity {
    void create(Entity e) {
        m_entityId = e.m_entityId;
        m_scene = e.m_scene;

        onCreate();
    }

    virtual void onCreate() {}
    virtual void onDestroy() {}
    virtual void onEvent(Event event) {}
    virtual void onUpdate(float elapsedTime) {}
};

} // namespace age
