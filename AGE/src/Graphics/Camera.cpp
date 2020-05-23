#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Camera.hpp"
#include "Viewport.hpp"

namespace age {

void Camera::create() {
    m_buffer.create(
        UniformBufferCreateInfo().setSize(sizeof(glm::mat4))
    );

    m_descriptor.get(
        DescriptorInfo().addBuffersBinding(
            BuffersBinding().addBuffer(m_buffer).setStage(VK_SHADER_STAGE_VERTEX_BIT)
        )
    );
}

void Camera::destroy() {
    m_buffer.destroy();
}

void Camera::setOrthoganalProjection(const Viewport& viewport, float zNear, float zFar) {
    m_projection = glm::ortho(
		0.0f, viewport.width,
		viewport.height, 0.0f,
        zNear, zFar
	);
}

void Camera::setPerspectiveProjection(const Viewport& viewport, float angle, float zNear, float zFar) {
    m_projection = glm::perspective(
        angle, viewport.width / viewport.height, zNear, zFar
    );
}

void Camera::upload() {
    auto mat = m_projection * getTransform();
    m_buffer.load(&mat, sizeof(mat));
}

} // namespace age
