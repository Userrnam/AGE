#pragma once

// #include <vector>
#include <unordered_map>

#include "UIBlock.hpp"
#include "../Events/Event.hpp"

namespace age {

class UIManager {
    static std::unordered_map<uint64_t, UIBlock> m_blocks;
    static uint64_t m_id;
public:
    static uint64_t addBlock(const UIBlock& b);

    static inline UIBlock& getBlock(uint64_t id) { return m_blocks[id]; }
    static inline void eraseBlock(uint64_t id) {
        auto it = m_blocks.find(id);
        if (it != m_blocks.end()) {
            it->second.destroy();
            m_blocks.erase(id);
        }
    }

    static bool update(Event e);
};

} // namespace age
