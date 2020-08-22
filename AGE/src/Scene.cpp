#include <chrono>

#include "Scene.hpp"

namespace age {

void Scene::update(float elapsedTime) {
    onUpdate(elapsedTime);

    float currentTime = std::chrono::duration<float, std::chrono::seconds::period>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    auto entities = m_registry.view<ScriptComponent*>();
    for (auto entity : entities) {
        auto script = entities.get(entity);
        script->onUpdate(elapsedTime);
    }

    // update views
    for (auto view : m_views) {
        view.update(elapsedTime, currentTime);
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
    auto entities = m_registry.view<ScriptComponent*>();
    for (auto entity : entities) {
        auto script = entities.get(entity);
        script->onDestroy();
    }

    onDestroy();

    for (auto& view : m_views) {
        view.destroy();
    }
    m_registry.clear();
}

} // namespace age
