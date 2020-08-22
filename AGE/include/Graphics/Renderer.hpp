#pragma once

#include <vector>

#include "Drawable.hpp"

namespace age {

class RenderPack {
    VkViewport m_viewport;
    const std::vector<Drawable>& m_targets;
    friend class Renderer;
public:
    RenderPack(const View& view, const std::vector<Drawable>& targets)
        : m_viewport(view.getViewport()), m_targets(targets) {}
};

class Renderer {
    friend class Application;
    static void create();
    static void destroy();
public:
    static void render(const std::vector<RenderPack>& packs);
};

} // namespace age
