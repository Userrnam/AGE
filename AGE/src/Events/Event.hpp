#pragma once

#include <unistd.h>

#include "Utils/utils.hpp"

namespace age {

class Event {
    uint64_t eventId;
    void *m_structure; // used for event data, should be pointing somewhere in arena alocator

    void setStructure(void* p, size_t size);
public:
    void setId(uint64_t id) {
        eventId = id;
    }

    template<typename T>
    void setStructure(T& s) {
        setStructure(&s, sizeof(T));
    }

    template<typename T>
    T& getStructure() {
        return *((T*)m_structure);
    }

    operator uint64_t() const {
        return eventId;
    }
};

namespace event {

const uint64_t KEY = hash("glfw_key");
const uint64_t SCROLL = hash("glfw_scroll");
const uint64_t CURSOR_POS = hash("glfw_cursor_pos");
const uint64_t MOUSE_BUTTON = hash("glfw_mouse_button");
const uint64_t RESIZE = hash("glfw_resize");

struct Key {
    int key;
    int scancode;
    int action;
    int mods;
};

struct Scroll {
    double xOffset;
    double yOffset;
};

struct CursorPos {
    double x;
    double y;
};

struct MouseButton {
    double xPos;
    double yPos;
    int button;
    int action;
    int mods;
};

struct Resize {
    int width;
    int height;
};

} // namespace event

} // namespace age
