#include <glm/gtc/matrix_transform.hpp>

#include "View.hpp"
#include "Core/Core.hpp"

namespace age {

View::View() {
    m_viewport.x = 0.0f;
    m_viewport.y = 0.0f;
    m_viewport.width = static_cast<float>(core::apiCore.swapchain.extent.width);
    m_viewport.height = static_cast<float>(core::apiCore.swapchain.extent.height);

    m_camera.setOrthoganalProjection(*this);
    m_camera.setOrigin({ m_viewport.width / 2, m_viewport.height / 2 });
    m_camera.setPosition({ m_viewport.width / 2, m_viewport.height / 2 });
    m_camera.upload();
}

} // namespace age
