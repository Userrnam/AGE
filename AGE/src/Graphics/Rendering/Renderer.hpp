#pragma once

#include <vector>

#include "ObjectCreation/Drawable.hpp"

namespace age {

class Renderer {
    friend class Application;
    static void create();
    static void destroy();

    static std::vector<Drawable> m_previousTargets;
public:
    static void render(const std::vector<Drawable>& targets);
    static void rerender() { render(m_previousTargets); }
};

} // namespace age
