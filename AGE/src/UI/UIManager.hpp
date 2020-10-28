#pragma once

// #include <vector>
#include <unordered_map>

#include "UIBlock.hpp"
#include "../Events/Event.hpp"

namespace age {

class UIManager {
    std::unordered_map<uint64_t, UIBlock> m_blocks;
    uint64_t m_id = 0;
public:
    inline UIBlock& getBlock(uint64_t id) { return m_blocks[id]; }

    uint64_t addBlock(const UIBlock& b);

    void eraseBlock(uint64_t id);
    bool update(Event e);
    void destroy();
};

} // namespace age
