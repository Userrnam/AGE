#include "Scene.hpp"

namespace age {

Scene::Scene(class Application* app) {
    parent = app;
}

Scene::~Scene() {
    {
        auto entities = m_registry.view<ScriptComponent*>();
        for (auto entity : entities) {
            auto script = entities.get(entity);
            delete script;
        }
    }

    m_registry.clear();
}

} // namespace age
