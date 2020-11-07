#include "UIManager.hpp"

#include "../Events/Event.hpp"

namespace age {

uint64_t UIManager::addBlock(const UIBlock& block) {
    m_id++;
    m_blocks[m_id] = block;

    return m_id;
}

void UIManager::eraseBlock(uint64_t id) {
    auto it = m_blocks.find(id);

    if (it != m_blocks.end()) {
        it->second.destroy();
        m_blocks.erase(id);
    }
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

void UIManager::destroy() {
    for (auto block : m_blocks) {
        block.second.destroy();
    }
}

} // namespace age
