#pragma once

#include "../Math/Vector.hpp"

namespace age {

struct Cursor {
    static Vector2f getPosition();
    static void setPosition(const Vector2f& position);
};

} // namespace age
