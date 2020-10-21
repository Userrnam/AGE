#pragma once

#include "../Events/Event.hpp"
#include "Entity.hpp"

namespace age {

struct StaticScriptComponent : public Entity {
    StaticScriptComponent(Entity e)
        : Entity(e.m_scene, e.m_entityId) {}

    virtual ~StaticScriptComponent() {}
};

struct ScriptComponent : public StaticScriptComponent {
    ScriptComponent(Entity e) : StaticScriptComponent(e) {}
    virtual void onEvent(Event event) {}
    virtual void onUpdate(float elapsedTime) {}
};

} // namespace age
