#pragma once

#include <unistd.h>
#include <stdint.h>

namespace age {

constexpr uint64_t hash(const char* str) {
    uint64_t hash = 5381;
    int c = 0;

    while ((c = *str)) {
        hash = ((hash << 5) + hash) + c;
        str++;
    }

    return hash;
}

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
    double xPos;
    double yPos;
};

struct MouseButton {
    double xPos;
    double yPos;
    int button;
    int action;
    int mods;
};

} // namespace event

} // namespace age
