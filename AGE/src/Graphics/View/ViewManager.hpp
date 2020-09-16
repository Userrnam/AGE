#pragma once

#include <unordered_map>
#include <stdint.h>

#include "View.hpp"

namespace age {

class ViewManager {
    static std::unordered_map<uint64_t, View> m_views;
    static View m_selectedView;
public:
    static void destroy();
    static void add(const age::View& view, uint64_t id);
    static const age::View& getSelected();
    static void select(uint64_t id);
    static void updateViews(float elapsedTime, float currentTime);
    static View& getView(uint64_t id);
};

} // namespace age
