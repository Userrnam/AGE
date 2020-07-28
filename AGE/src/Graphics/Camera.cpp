#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Camera.hpp"
#include "Viewport.hpp"

namespace age {

void Camera::create() {
    m_buffer.create(
        UniformBufferCreateInfo().setSize(sizeof(CameraUniform))
    );

    m_descriptor.get(
        DescriptorSetInfo().addBinding(
            DescriptorBinding()
            .add(m_buffer)
            .setStage(VK_SHADER_STAGE_VERTEX_BIT)
            .setDescriptorType(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
        )
    );
}

void Camera::destroy() {
    m_buffer.destroy();
}

void Camera::setOrthoganalProjection(const Viewport& viewport, float zNear, float zFar) {
    m_uniform.m_time[2] = viewport.width;
    m_uniform.m_time[3] = viewport.height;

    m_uniform.m_projection = glm::ortho(
		0.0f, viewport.width,
		viewport.height, 0.0f,
        zNear, zFar
	);
    m_projectionChanged = true;
}

void Camera::setPerspectiveProjection(const Viewport& viewport, float angle, float zNear, float zFar) {
    m_uniform.m_projection = glm::perspective(
        angle, viewport.width / viewport.height, zNear, zFar
    );
    m_projectionChanged = true;
}

void Camera::upload() {
    if (m_projectionChanged) {
        CameraUniform u;
        u.m_projection = m_uniform.m_projection * getTransform();
        u.m_time = m_uniform.m_time;
        m_buffer.load(&u, sizeof(CameraUniform));
    } else {
        m_buffer.load(&m_uniform.m_time, sizeof(m_uniform.m_time), offsetof(CameraUniform, m_time));
    }
}

} // namespace age
