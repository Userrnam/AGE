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

    if (viewport.width == 0 || viewport.height == 0) {
        m_viewport.x = 0.0f;
        m_viewport.y = 0.0f;
        m_viewport.width = static_cast<float>(core::apiCore.swapchain.extent.width);
        m_viewport.height = static_cast<float>(core::apiCore.swapchain.extent.height);
    } else {
        m_viewport = viewport;
    }

    camera.setOrthoganalProjection(m_viewport);
    setOrigin({ m_viewport.width / 2, m_viewport.height / 2 });
    setPosition({ m_viewport.width / 2, m_viewport.height / 2 });

    m_globals.cameraTransform = camera.getProjection() * getTransform();
    m_globals.resolution = glm::vec2(m_viewport.width, m_viewport.height);

    m_buffer.load(&m_globals, sizeof(m_globals));
}

void View::update(float elapsedTime, float currentTime) {
    m_globals.deltaTime = elapsedTime;
    m_globals.time = currentTime;
    if (Transformable::needUpdate()) {
        m_globals.cameraTransform = camera.getProjection() * Transformable::getTransform();
    }
    m_buffer.load(&m_globals, sizeof(ViewGlobals));
}

void View::destroy() {
    m_buffer.destroy();
}

} // namespace age
