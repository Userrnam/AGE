#pragma once

// #include <vector>
#include <unordered_map>

#include "UIBlock.hpp"
#include "../Events/Event.hpp"
#include "../Events/EventManager.hpp"

namespace age {

class UIManager {
    EVENT_CALLBACK(UIManager, cursorMoved);
    EVENT_CALLBACK(UIManager, mouseButtonPressed);

    std::unordered_map<uint64_t, UIBlock> m_blocks;
    uint64_t m_id = 0;
public:
    inline UIBlock& getBlock(uint64_t id) { return m_blocks[id]; }

    uint64_t addBlock(const UIBlock& b);

    void eraseBlock(uint64_t id);

    void cursorMoved(const event::CursorPos& e);
    void mouseButtonPressed(const event::MouseButton& e);

    void destroy();
};

} // namespace age
