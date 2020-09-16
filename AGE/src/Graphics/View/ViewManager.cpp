#include <unordered_map>

#include "ViewManager.hpp"

namespace age {

std::unordered_map<uint64_t, View> ViewManager::m_views;
View ViewManager::m_selectedView;

void ViewManager::destroy() {
    for (auto& p : m_views) {
        p.second.destroy();
    }
}

void ViewManager::add(const age::View& view, uint64_t id) {
    m_views[id] = view;
    m_views[id].m_id = id;
}

const View& ViewManager::getSelected() {
    return m_selectedView;
}

void ViewManager::select(uint64_t id) {
    m_selectedView = m_views[id];
}

void ViewManager::updateViews(float elapsedTime, float currentTime) {
    for (auto& p : m_views) {
        p.second.m_globals.deltaTime = elapsedTime;
        p.second.m_globals.time = currentTime;
        p.second.m_buffer.load(&p.second.m_globals, sizeof(View::ViewGlobals));
    }
}

View& ViewManager::getView(uint64_t id) {
    return m_views[id];
}

} // namespace age::ViewManager