#pragma once

#include <vector>

#include "UIBlock.hpp"
#include "Events.hpp"

namespace age {

class UIManager {
    static std::vector<UIBlock> m_blocks;
public:
    static inline void addBlock(const UIBlock& b) { m_blocks.push_back(b); }

    static bool update(Event e);
};

} // namespace age
