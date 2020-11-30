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

void UIManager::cursorMoved(const event::CursorPos &e) {
    for (auto& block : m_blocks) {
        if (block.second.update(e)) {
            return;
        }
    }
}

void UIManager::mouseButtonPressed(const event::MouseButton &e) {
    for (auto& block : m_blocks) {
        if (block.second.update(e)) {
            return;
        }
    }
}

void UIManager::destroy() {
    for (auto block : m_blocks) {
        block.second.destroy();
    }
}

} // namespace age
