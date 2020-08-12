#include <chrono>

#include "Scene.hpp"

namespace age {

void Scene::update(float elapsedTime) {
    onUpdate(elapsedTime);

    float currentTime = std::chrono::duration<float, std::chrono::seconds::period>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    // update views
    for (auto view : m_views) {
        view.update(elapsedTime, currentTime);
    }
}

} // namespace age
