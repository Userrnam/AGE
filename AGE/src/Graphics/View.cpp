#include <glm/gtc/matrix_transform.hpp>

#include "View.hpp"
#include "Core/Core.hpp"

namespace age {

View::View() {
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(core::apiCore.swapchain.extent.width);
    viewport.height = static_cast<float>(core::apiCore.swapchain.extent.height);

    camera.setOrthoganalProjection(*this);
    camera.setOrigin({ viewport.width / 2, viewport.height / 2 });
    camera.setPosition({ viewport.width / 2, viewport.height / 2 });
    camera.upload();
}

} // namespace age
