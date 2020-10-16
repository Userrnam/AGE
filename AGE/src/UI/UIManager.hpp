#pragma once

// #include <vector>
#include <unordered_map>

#include "UIBlock.hpp"
#include "Events.hpp"

namespace age {

class UIManager {
    static std::unordered_map<uint64_t, UIBlock> m_blocks;
    static uint64_t m_id;
public:
    static uint64_t addBlock(const UIBlock& b);

    static inline UIBlock& getBlock(uint64_t id) { return m_blocks[id]; }
    static inline void eraseBlock(uint64_t id) {
        if (m_blocks.find(id) != m_blocks.end()) {
            m_blocks.erase(id);
        }
    }

    static bool update(Event e);
};

} // namespace age
