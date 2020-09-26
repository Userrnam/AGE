#include "UIManager.hpp"

#include "Events.hpp"

namespace age {

std::vector<UIBlock> UIManager::m_blocks;

bool UIManager::update(Event e) {
    if (e == event::MOUSE_BUTTON) {
        auto s = e.getStructure<event::MouseButton>();
        for (auto& block : m_blocks) {
            if (block.update(s)) {
                return true;
            }
        }
        return true;
    } else if (e == event::CURSOR_POS) {
        auto s = e.getStructure<event::CursorPos>();
        for (auto& block : m_blocks) {
            if (block.update(s)) {
                return true;
            }
        }
    }
    return false;
}

} // namespace age
