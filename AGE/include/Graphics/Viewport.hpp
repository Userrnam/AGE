#pragma once

namespace age {

// for some reason origin is top left
struct Viewport {
    float x = 0, y = 0;
    float width = 0, height = 0;

    Viewport() {}

    Viewport(float _x, float _y, float _width, float _height) {
        x = _x;
        y = _y;
        width = _width;
        height = _height;
    }
};

} // namespace age
