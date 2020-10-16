#include "UIManager.hpp"

#include "Events.hpp"

namespace age {

std::unordered_map<uint64_t, UIBlock> UIManager::m_blocks;
uint64_t UIManager::m_id;

uint64_t UIManager::addBlock(const UIBlock& block) {
    m_id++;
    m_blocks[m_id] = block;

    return m_id;
}

bool UIManager::update(Event e) {
    if (e == event::MOUSE_BUTTON) {
        auto s = e.getStructure<event::MouseButton>();
        for (auto& block : m_blocks) {
            if (block.second.update(s)) {
                return true;
            }
        }
        return true;
    } else if (e == event::CURSOR_POS) {
        auto s = e.getStructure<event::CursorPos>();
        for (auto& block : m_blocks) {
            if (block.second.update(s)) {
                return true;
            }
        }
    }
    return false;
}

} // namespace age
