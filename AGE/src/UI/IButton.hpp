#pragma once

#include "Math.hpp"

namespace age {

class IButton {
protected:
    Vector2f m_pos1;
    Vector2f m_pos2;

    void updatePoints(const Vector2f& pos, const Vector2f& size) {
        m_pos1 = pos;
        m_pos2 = pos + size;
    }

public:
    virtual ~IButton() {}

    inline bool isCovered(float x, float y) {
        return (x > m_pos1.x && y > m_pos1.y) && (x < m_pos2.x && y < m_pos2.y);
    }

    virtual void onEnter() {}
    virtual void onLeave() {}
    virtual void onPress(unsigned button) {}

    virtual void setPosition(const Vector2f& pos) = 0;
    virtual void setSize(const Vector2f& size) = 0;

    inline void setPosition(float x, float y) { setPosition({x, y}); }
    inline void setSize(float x, float y) { setSize({x, y}); }

    inline void move(const Vector2f& v) {
        m_pos1 += v;
        m_pos2 += v;
    }

    inline void move(float x, float y) { move({x, y}); }

    inline Vector2f getPosition() {
        return m_pos1;
    }

    inline Vector2f getSize() {
        return m_pos2 - m_pos1;
    }
};

} // namespace age
