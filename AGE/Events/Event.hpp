#pragma once

#include <unistd.h>

#include "../Utils/utils.hpp"
#include "../Math/Vector.hpp"

namespace age::event {

struct Update {
    float elapsedTime;
    float runTime;
};

struct Key {
    int key;
    int scancode;
    int action;
    int mods;
};

struct Scroll {
    Vector2f offset;
};

struct CursorPos {
    Vector2f pos;
};

struct MouseButton {
    Vector2f pos;
    int button;
    int action;
    int mods;
};

struct Resize {
    Vector2i oldSize;
    Vector2i newSize;
};

} // namespace age::event
