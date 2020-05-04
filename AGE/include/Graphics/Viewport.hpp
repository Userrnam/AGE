#pragma once


namespace age {

struct Viewport {
    float x;
    float y;
    float width;
    float height;
};

void setViewport(const Viewport& viewport);
void setDefaultViewport();
    
} // namespace age

