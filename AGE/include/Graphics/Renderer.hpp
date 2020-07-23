#pragma once

#include <vector>

#include "Drawable.hpp"

namespace age {

class Renderer {
    std::vector<Drawable>* pTargets = nullptr;

    // TODO: update command buffers only if needed

    void updateCommandBuffers();
public:
    void init();
    void destroy();
    void draw(int i);

    void render(std::vector<Drawable>& targets);
};

} // namespace age
