#pragma once

#include "Events/Event.hpp"
#include "Scene/Entity.hpp"

namespace age {

struct StaticScriptComponent : public Entity {
    StaticScriptComponent(Entity e) {
        m_entityId = e.m_entityId;
        m_scene = e.m_scene;
    }

    virtual ~StaticScriptComponent() {}
};

struct ScriptComponent : public StaticScriptComponent {
    ScriptComponent(Entity e) : StaticScriptComponent(e) {}
    virtual void onEvent(Event event) {}
    virtual void onUpdate(float elapsedTime) {}
};

} // namespace age
