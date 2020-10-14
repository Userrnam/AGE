#pragma once

#include <vector>
#include <stdint.h>
#include <iostream>

#include "IButton.hpp"
#include "Events.hpp"

#define ps() std::cout << __LINE__ << " cb " << m_coveredButton << "\n"

namespace age {

// alignment is relative to first button
class UIBlock {
    std::vector<IButton*> m_buttons;
    // Fixme:
    // have no idea why but without this it crashes
    int dummy;
    IButton* m_coveredButton = nullptr;

public:
    inline UIBlock& addButton(IButton* b) { m_buttons.push_back(b); return *this; }

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
