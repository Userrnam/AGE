#pragma once

#include <vector>

#include "External/entt.hpp"

#include "ObjectCreation/Drawable.hpp"

namespace age {

class Renderer {
    friend class Application;
    static void create();
    static void destroy();

    static std::vector<Drawable> m_previousTargets;
    static std::vector<entt::entity> m_previousTargetsIds;
public:
    static bool renderRequired(const std::vector<entt::entity>& ids);
    static void render(const std::vector<Drawable>& targets);
    static void rerender() { render(m_previousTargets); }
};

} // namespace age
