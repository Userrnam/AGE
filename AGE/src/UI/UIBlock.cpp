#include <GLFW/glfw3.h>

#include "UIBlock.hpp"

namespace age {

bool UIBlock::update(const event::MouseButton& e) {
    for (size_t i = 0; i < m_buttons.count(); ++i) {
        auto b = m_buttons[i];
        if (b->m_hitbox.isCovered(Vector2f(e.xPos, e.yPos))) {
            if (!m_coveredButton) {
                m_coveredButton = b;
                b->onEnter();
            } else if (m_coveredButton != b) {
                m_coveredButton->onLeave();
                m_coveredButton = b;
                b->onEnter();
            }
            if (e.action == GLFW_PRESS) {
                b->onPress(e.button);
            }
            return true;
        }
    }
    return false;
}

bool UIBlock::update(const event::CursorPos& p) {
    for (int i = 0; i < m_buttons.count(); ++i) {
        IButton* b = m_buttons[i];
        if (b->m_hitbox.isCovered(Vector2f(p.x, p.y))) {
            if (!m_coveredButton) {
                m_coveredButton = b;
                b->onEnter();
            } else if (m_coveredButton != b) {
                    m_coveredButton->onLeave();
                    m_coveredButton = b;
                    b->onEnter();
            }
            return true;
        }
    }
    if (m_coveredButton) {
        m_coveredButton->onLeave();
        m_coveredButton = nullptr;
    }
    return false;
}

UIBlock& UIBlock::alignVertically(float spacing) {
    for (size_t i = 1; i < m_buttons.count(); ++i) {
        auto prevButtonY = m_buttons[i-1]->m_hitbox.getPosition().y;
        auto prevButtonSize = m_buttons[i-1]->m_hitbox.getSize();
        m_buttons[i]->__move({0, prevButtonSize.y + prevButtonY + spacing});
    }
    return *this;
}

UIBlock& UIBlock::alignHorizontally(float spacing) {
    for (size_t i = 1; i < m_buttons.count(); ++i) {
        auto prevButtonX = m_buttons[i-1]->m_hitbox.getPosition().x;
        auto prevButtonSize = m_buttons[i-1]->m_hitbox.getSize();
        m_buttons[i]->__move({prevButtonX + prevButtonSize.x + spacing, 0});
    }
    return *this;
}

UIBlock& UIBlock::move(const Vector2f& v) {
    for (size_t i = 0; i < m_buttons.count(); ++i) {
        m_buttons[i]->__move(v);
    }
    return *this;
}

UIBlock& UIBlock::setPosition(const Vector2f& v) {
    auto firstPos = m_buttons[0]->m_hitbox.getPosition();
    return this->move(v - firstPos);
}

} // namespace age
