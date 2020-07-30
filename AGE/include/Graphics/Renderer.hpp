#pragma once

#include <vector>

#include "Drawable.hpp"

namespace age {

class Renderer {
    std::vector<Drawable>* pTargets = nullptr;

    // TODO: update command buffers only if needed

    void updateCommandBuffers();

    void draw(int i);

    friend class Application;
public:
    void create();
    void destroy();
    void render(std::vector<Drawable>& targets);
};

} // namespace age
