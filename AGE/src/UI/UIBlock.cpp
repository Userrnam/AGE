#include "UIBlock.hpp"

namespace age {

bool UIBlock::update(const event::MouseButton& e) {
    for (size_t i = 0; i < m_buttons.count(); ++i) {
        auto b = m_buttons[i];
        if (b->isCovered(e.xPos, e.yPos)) {
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
        if (b->isCovered(p.x, p.y)) {
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
    auto firstX = m_buttons[0]->getPosition().y;
    for (size_t i = 1; i < m_buttons.count(); ++i) {
        auto prevButtonY = m_buttons[i-1]->getPosition().y;
        auto prevButtonSize = m_buttons[i-1]->getSize();
        m_buttons[i]->setPosition(firstX, prevButtonSize.y + prevButtonY + spacing);
    }
    return *this;
}

UIBlock& UIBlock::alignHorizontally(float spacing) {
    auto firstY = m_buttons[0]->getPosition().y;
    for (size_t i = 1; i < m_buttons.count(); ++i) {
        auto prevButtonX = m_buttons[i-1]->getPosition().x;
        auto prevButtonSize = m_buttons[i-1]->getSize();
        m_buttons[i]->setPosition(prevButtonX + prevButtonSize.x + spacing, firstY);
    }
    return *this;
}

UIBlock& UIBlock::move(const Vector2f& v) {
    for (size_t i = 0; i < m_buttons.count(); ++i) {
        m_buttons[i]->move(v);
    }
    return *this;
}

UIBlock& UIBlock::setPosition(const Vector2f& v) {
    auto firstPos = m_buttons[0]->getPosition();
    return this->move(v - firstPos);
}

} // namespace age
