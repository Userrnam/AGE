#pragma once

#include <glm/glm.hpp>

namespace age {

struct Cursor {
    static glm::vec2 getPosition();
    static void setPosition(const glm::vec2& position);
};

} // namespace age
