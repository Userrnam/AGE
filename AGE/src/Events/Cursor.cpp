#include "Cursor.hpp"
#include "Graphics/Core/Core.hpp"

namespace age {

Vector2f Cursor::getPosition() {
    double x, y;
    glfwGetCursorPos(core::apiCore.window.handle, &x, &y);

    return {(float)x, (float)(core::apiCore.window.height - y)};
}

void Cursor::setPosition(const Vector2f& pos) {
    glfwSetCursorPos(core::apiCore.window.handle, pos.x, pos.y);
}

} // namespace age
