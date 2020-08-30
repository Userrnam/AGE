#pragma once

#include <vector>

#include "Drawable.hpp"

namespace age {

class Renderer {
    friend class Application;
    static void create();
    static void destroy();
public:
    static void render(const std::vector<Drawable>& targets);
};

} // namespace age
