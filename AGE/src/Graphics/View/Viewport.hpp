#pragma once

#include <vulkan/vulkan.h>

namespace age {

// for some reason origin is top left
struct Viewport : public VkViewport {
    Viewport() {
        x = 0;
        y = 0;
        width = 0;
        height = 0;
        minDepth = 0;
        maxDepth = 0;
    }
    Viewport(float _x, float _y, float _width, float _height) {
        x = _x;
        y = _y;
        width = _width;
        height = _height;
        minDepth = 0;
        maxDepth = 0;
    }
};

} // namespace age
