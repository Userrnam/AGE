#pragma once

#include <glm/glm.hpp>

namespace age {

class IButton {
protected:
    glm::vec2 m_pos1;
    glm::vec2 m_pos2;

    void updatePoints(const glm::vec2& pos, const glm::vec2& size) {
        m_pos1 = pos;
        m_pos2 = pos + size;
    }

public:
    inline bool isCovered(float x, float y) {
        return (x > m_pos1.x && y > m_pos1.y) && (x < m_pos2.x && y < m_pos2.y);
    }

    virtual void onEnter() {}
    virtual void onLeave() {}
    virtual void onPress(unsigned button) {}

    virtual void setPosition(const glm::vec2& pos) = 0;
    virtual void setSize(const glm::vec2& size) = 0;

    inline glm::vec2 getPosition() {
        return m_pos1;
    }

    inline glm::vec2 getSize() {
        return m_pos2 - m_pos1;
    }
};

} // namespace age
