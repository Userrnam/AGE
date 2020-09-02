#pragma once

#include <string>
#include <unordered_map>

#include "View.hpp"

namespace age {

class ViewManager {
    static std::unordered_map<std::string, View> m_views;
    static View m_selectedView;
public:
    static void destroy();
    static void add(const age::View& view, const std::string& viewName);
    static const age::View& getSelected();
    static void select(const std::string viewName);
    static void updateViews(float elapsedTime, float currentTime);
};

} // namespace age
