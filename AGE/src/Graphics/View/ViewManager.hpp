#pragma once

#include <unordered_map>
#include <stdint.h>

#include "View.hpp"
#include "Math.hpp"

namespace age {

class ViewManager {
    static std::unordered_map<uint64_t, View> m_views;
    static Vector2i m_prevWindowSize;

    static View m_selectedView;
public:
    static void destroy();
    static void add(const age::View& view, uint64_t id);
    static void updateViews(float elapsedTime, float currentTime);

    inline static void select(uint64_t id) { m_selectedView = m_views[id]; }
    inline static View& getView(uint64_t id) { return m_views[id]; }
    inline static age::View& getSelected() { return m_selectedView; }
    inline static std::unordered_map<uint64_t, View> getViews() { return m_views; }
};

} // namespace age
