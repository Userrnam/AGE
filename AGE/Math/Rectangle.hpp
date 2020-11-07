#pragma once

#include "Vector.hpp"

namespace age {

struct Rectangle {
    Vector2f pos; // bottom left
    Vector2f size;

    bool intersect(const Rectangle& p) {
        float left = std::min(pos.x, p.pos.x);
        float right = std::max(pos.x + size.x, p.pos.x + p.size.x);
        float bottom = std::min(pos.y, p.pos.y);
        float top = std::max(pos.y + size.y, p.pos.y + p.size.y);

        return right - left < size.x + p.size.x && top - bottom < size.y + p.size.y;
    }
};

} // namespace age
