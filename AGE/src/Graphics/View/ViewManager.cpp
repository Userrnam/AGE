#include <unordered_map>

#include "ViewManager.hpp"
#include "../Core/Core.hpp"

namespace age {

std::unordered_map<uint64_t, View> ViewManager::m_views;
View ViewManager::m_selectedView;
Vector2i ViewManager::m_prevWindowSize;

void ViewManager::destroy() {
    for (auto& p : m_views) {
        p.second.destroy();
    }
}

void ViewManager::add(const age::View& view, uint64_t id) {
    m_views[id] = view;
    m_views[id].m_id = id;
}

void ViewManager::updateViews(float elapsedTime, float currentTime) {
    bool windowResized = false;
    if (m_prevWindowSize.x != core::apiCore.window.width || m_prevWindowSize.y != core::apiCore.window.height) {
        windowResized = true;

        m_prevWindowSize.x = core::apiCore.window.width;
        m_prevWindowSize.y = core::apiCore.window.height;
    }

    for (auto& p : m_views) {
        if (windowResized) {
            p.second.m_camera.setOrthoganalProjection(core::apiCore.window.width, core::apiCore.window.height);
            p.second.updateCameraTransform();
        }

        p.second.m_globals.deltaTime = elapsedTime;
        p.second.m_globals.time = currentTime;
        p.second.m_buffer.load(&p.second.m_globals, sizeof(View::ViewGlobals));
    }
}

} // namespace age::ViewManager