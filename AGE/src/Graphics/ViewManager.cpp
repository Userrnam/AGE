#include <unordered_map>

#include "ViewManager.hpp"

namespace age {

std::unordered_map<std::string, View> ViewManager::m_views;
View ViewManager::m_selectedView;

void ViewManager::destroy() {
    for (auto& p : m_views) {
        p.second.destroy();
    }
}

void ViewManager::add(const age::View& view, const std::string& viewName) {
    m_views[viewName] = view;
}

const View& ViewManager::getSelected() {
    return m_selectedView;
}

void ViewManager::select(const std::string viewName) {
    m_selectedView = m_views[viewName];
}

void ViewManager::updateViews(float elapsedTime, float currentTime) {
    for (auto& p : m_views) {
        p.second.m_globals.deltaTime = elapsedTime;
        p.second.m_globals.time = currentTime;
        p.second.m_buffer.load(&p.second.m_globals, sizeof(View::ViewGlobals));
    }
}

} // namespace age::ViewManager