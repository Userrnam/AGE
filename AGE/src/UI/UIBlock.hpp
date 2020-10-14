#pragma once

#include <vector>
#include <stdint.h>
#include <iostream>

#include "IButton.hpp"
#include "Events.hpp"

#include "Containers/DynamicArray.hpp"

namespace age {

// alignment is relative to first button
class UIBlock {
    DynamicArray<IButton*> m_buttons;
    IButton* m_coveredButton = nullptr;

public:
    void create(size_t buttonCount) {
        m_buttons.create(buttonCount);
    }

    void destroy() {
        for (size_t i = 0; i < m_buttons.count(); ++i) {
            delete m_buttons[i];
        }
        m_buttons.destroy();
    }

    inline UIBlock& addButton(IButton* b) { m_buttons.add(b); return *this; }

    bool update(const event::MouseButton& e);
    bool update(const event::CursorPos& p);

    UIBlock& alignVertically(float spacing = 0);
    UIBlock& alignHorizontally(float spacing = 0);

    UIBlock& move(float x, float y) { return move({x, y}); }
    UIBlock& setPosition(float x, float y) { return setPosition({x, y}); }

    UIBlock& move(const Vector2f& v);
    UIBlock& setPosition(const Vector2f& v);
};

} // namespace age
