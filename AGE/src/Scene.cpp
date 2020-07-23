#include <chrono>

#include "Scene.hpp"

namespace age {

void Scene::update(float elapsedTime) {
    // update cameras
    for (auto view : m_views) {
        view.camera.m_uniform.m_time[0] = std::chrono::duration<float, std::chrono::seconds::period>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        view.camera.upload();
    }
}

} // namespace age
