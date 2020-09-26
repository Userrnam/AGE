#include "Cursor.hpp"
#include "Graphics/Core/Core.hpp"

namespace age {

glm::vec2 Cursor::getPosition() {
    double x, y;
    glfwGetCursorPos(core::apiCore.window.handle, &x, &y);

    return {x, core::apiCore.window.height - y};
}

void Cursor::setPosition(const glm::vec2& pos) {
    glfwSetCursorPos(core::apiCore.window.handle, pos.x, pos.y);
}

} // namespace age
