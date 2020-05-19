#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Camera.hpp"

#include "View.hpp"

namespace age {

Camera::Camera() {
    m_buffer.create(
        core::BufferCreateInfo()
            .setMemoryProperties(VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
            .setUsage(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
            .setSize(sizeof(glm::mat4))
    );

    m_descriptor.get(
        DescriptorInfo().addBuffer(m_buffer)
    );
}

Camera::~Camera() {
    m_buffer.destroy();
}

void Camera::setOrthoganalProjection(View& view, float zNear, float zFar) {
    m_projection = glm::ortho(
		0.0f, view.getViewport().width,
		view.getViewport().height, 0.0f,
        zNear, zFar
	);
}

void Camera::setPerspectiveProjection(View& view, float angle, float zNear, float zFar) {
    m_projection = glm::perspective(
        angle, view.getViewport().width / view.getViewport().height, zNear, zFar
    );
}

void Camera::upload() {
    auto mat = m_projection * getTransform();
    m_buffer.loadData(&mat, sizeof(mat));
}

} // namespace age
