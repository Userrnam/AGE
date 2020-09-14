#include <chrono>

#include "Scene.hpp"

namespace age {

void Scene::update(float elapsedTime) {
    onUpdate(elapsedTime);

    auto entities = m_registry.view<ScriptComponent*>();
    for (auto entity : entities) {
        auto script = entities.get(entity);
        script->onUpdate(elapsedTime);
    }
}

void Scene::handleEvent(Event event) {
    onEvent(event);

    auto entities = m_registry.view<ScriptComponent*>();
    for (auto entity : entities) {
        auto script = entities.get(entity);
        script->onEvent(event);
    }
}

void Scene::create(Application* app) {
    parent = app;

    onCreate();
}

void Scene::destroy() {
    {
        auto entities = m_registry.view<ScriptComponent*>();
        for (auto entity : entities) {
            auto script = entities.get(entity);
            delete script;
        }
    }

    {
        auto entities = m_registry.view<StaticScriptComponent*>();
        for (auto entity : entities) {
            auto script = entities.get(entity);
            delete script;
        }
    }

    onDestroy();

    m_registry.clear();
}

} // namespace age
