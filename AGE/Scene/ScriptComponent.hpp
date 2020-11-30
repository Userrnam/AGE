#pragma once

#include "../Events/Event.hpp"
#include "Entity.hpp"

namespace age {

struct ScriptComponent : public Entity {
    ScriptComponent(Entity e)
        : Entity((SceneBase*)e.getScene(), e.getEntityId()) {}

    virtual ~ScriptComponent() {}
};

} // namespace age
