#include <assert.h>

#include "View.hpp"
#include "Core/Core.hpp"
#include "Core/Command.hpp"

namespace age {


void View::updateCameraTransform() {
    m_globals.cameraTransform = m_camera.getProjection() * UnmanagedTransformable::getTransform();
}

void View::create(PositionManager* p) {
    m_positionManager = p;

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

    auto width  = static_cast<float>(core::apiCore.window.width);
    auto height = static_cast<float>(core::apiCore.window.height);

    m_camera.setOrthoganalProjection(width, height);
    setOrigin({ width / 2, height / 2 });
    setPosition({ width / 2, height / 2 });

    m_globals.cameraTransform = m_camera.getProjection() * UnmanagedTransformable::getTransform();
    m_globals.resolution = Vector2f(core::apiCore.swapchain.extent.width, core::apiCore.swapchain.extent.height);

    m_buffer.load(&m_globals, sizeof(m_globals));
}

void View::destroy() {
    m_buffer.destroy();
}

} // namespace age
