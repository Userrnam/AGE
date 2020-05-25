#include "Layer.hpp"
#include "Core/Core.hpp"
#include "Core/Command.hpp"

namespace age {

void Layer::clearWindow(int i, const glm::vec4 clearColor) {
    age::core::cmd::clear(i, clearColor);
}

Layer::Layer(const Viewport& viewport) {
    camera.create();

    if (viewport.width == 0 | viewport.height == 0) {
        m_viewport.x = 0.0f;
        m_viewport.y = 0.0f;
        m_viewport.width = static_cast<float>(core::apiCore.swapchain.extent.width);
        m_viewport.height = static_cast<float>(core::apiCore.swapchain.extent.height);
    } else {
        m_viewport = viewport;
    }

    camera.setOrthoganalProjection(m_viewport);
    camera.setOrigin({ m_viewport.width / 2, m_viewport.height / 2 });
    camera.setPosition({ m_viewport.width / 2, m_viewport.height / 2 });
    camera.upload();
}

void Layer::destroy() {
    onDestroy();
    camera.destroy();
}

void Layer::draw(int i) {
    core::cmd::clear(i, {});
}

} // namespace age
