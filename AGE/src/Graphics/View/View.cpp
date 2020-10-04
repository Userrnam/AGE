#include <assert.h>

#include "View.hpp"
#include "Core/Core.hpp"
#include "Core/Command.hpp"

namespace age {

void View::create(const Viewport& viewport) {
    m_buffer.create(
        UniformBufferCreateInfo()
        .setSize(sizeof(ViewGlobals))
    );

    m_descriptor.get(
        DescriptorSetInfo().addBinding(
            DescriptorBinding()
            .add(m_buffer)
            .setStage(VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT)
            .setDescriptorType(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
        )
    );
    
    setViewport(viewport);
}

void View::setViewport(const Viewport& viewport) {
    m_viewport = viewport;

    if (viewport.width == 0 || viewport.height == 0) {
        m_viewport.x = 0.0f;
        m_viewport.y = 0.0f;
        m_viewport.width = static_cast<float>(core::apiCore.swapchain.extent.width);
        m_viewport.height = static_cast<float>(core::apiCore.swapchain.extent.height);
    }

    Viewport camerav = {};
    camerav.width  = static_cast<float>(core::apiCore.window.width);
    camerav.height = static_cast<float>(core::apiCore.window.height);

    camera.setOrthoganalProjection(camerav);
    setOrigin({ camerav.width / 2, camerav.height / 2 });
    setPosition({ camerav.width / 2, camerav.height / 2 });

    m_globals.cameraTransform = camera.getProjection() * Transformable::getTransform();
    m_globals.resolution = glm::vec2(m_viewport.width, m_viewport.height);

    m_buffer.load(&m_globals, sizeof(m_globals));
}

void View::destroy() {
    m_buffer.destroy();
}

} // namespace age
