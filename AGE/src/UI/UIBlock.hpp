#pragma once

#include <vector>
#include <stdint.h>
#include <iostream>

#include "IButton.hpp"
#include "Events.hpp"

#define ps() std::cout << __LINE__ << " cb " << m_coveredButton << "\n"

namespace age {

class UIBlock {
    std::vector<IButton*> m_buttons;
    // Fixme:
    // have no idea why but without this it crashes
    int dummy;
    IButton* m_coveredButton = nullptr;

public:
    inline UIBlock& addButton(IButton* b) { m_buttons.push_back(b); return *this; }

    bool update(const event::MouseButton& e) {
        for (IButton* b : m_buttons) {
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

    bool update(const event::CursorPos& p) {
        for (int i = 0; i < m_buttons.size(); ++i) {
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
};

} // namespace age
