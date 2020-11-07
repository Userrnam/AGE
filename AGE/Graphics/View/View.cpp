#include <assert.h>

#include "View.hpp"
#include "../Core/Core.hpp"
#include "../Core/Command.hpp"

namespace age {

Vector2i maxSizeResizeHandler(Vector2i oldSize, Vector2i newSize) {
    return newSize;
}

Vector2i preserveWidthResizeHandler(Vector2i oldSize, Vector2i newSize) {
    Vector2i result;
    result.x = oldSize.x;
    result.y = newSize.y * oldSize.x / newSize.x;
    return result;
}

Vector2i preserveHeighthResizeHandler(Vector2i oldSize, Vector2i newSize) {
    Vector2i result;
    result.x = newSize.x * oldSize.y / newSize.y;
    result.y = oldSize.y;
    return result;
}

void View::updateCameraTransform() {
    m_globals.cameraTransform = m_camera.getProjection() * UnmanagedTransformable::getTransform();
}

void View::handleWindowResize(const Vector2i &oldSize, const Vector2i newSize) {
    auto s = m_resizeHandler(oldSize, newSize);
    m_camera.setOrthoganalProjection(s.x, s.y);
}

void View::update(float elapsedTime, float currentTime) {
    m_globals.deltaTime = elapsedTime;
    m_globals.time = currentTime;
    m_buffer.load(&m_globals, sizeof(View::ViewGlobals));
}

void View::create() {
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
    m_descriptor.destroy();
}

} // namespace age
