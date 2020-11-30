#pragma once

#include "../Math/Vector.hpp"

namespace age {

struct HitBox {
    Vector2f pos1 = 0;
    Vector2f pos2 = 0;

    bool isCovered(const Vector2f& pos) {
        return (pos.x > pos1.x && pos.y > pos1.y) && (pos.x < pos2.x && pos.y < pos2.y);
    }

    void update(const Vector2f& pos, const Vector2f& size) {
        pos1 = pos;
        pos2 =pos + size;
    }

    void move(const Vector2f& v) {
        pos1 += v;
        pos2 += v;
    }

    void setPosition(const Vector2f& pos) {
        this->move(pos - pos1);
    }

    void setSize(const Vector2f& size) {
        pos2 = pos1 + size;
    }

    Vector2f getPosition() {
        return pos1;
    }

    Vector2f getSize() {
        return pos2 - pos1;
    }
};

class IButton {
    HitBox m_hitbox;

    void __setSize(const Vector2f& size) {
        m_hitbox.setSize(size);
        setSize(size);
    }

    void __move(const Vector2f& v) {
        m_hitbox.move(v);
        move(v);
    }

    friend class UIBlock;
protected:
    void updateHitBox(const Vector2f& pos, const Vector2f& size) {
        m_hitbox.update(pos, size);
    }

public:
    virtual ~IButton() {}

    virtual void onEnter() {}
    virtual void onLeave() {}
    virtual void onPress(unsigned button) {}

    virtual void move(const Vector2f& v) = 0;
    virtual void setSize(const Vector2f& size) = 0;

    inline void move(float x, float y) { move({x, y}); }
    inline void setSize(float x, float y) { setSize({x, y}); }
};

} // namespace age
