#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Camera.hpp"

#include "View.hpp"

namespace age {

Camera::Camera() {
    core::BufferCreateInfo createInfo;
    createInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
    createInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    createInfo.size = sizeof(glm::mat4);
    m_buffer.create(createInfo);

    // create descriptor
    DescriptorInfo descriptorInfo;
    descriptorInfo.ubos.resize(1);
    descriptorInfo.ubos[0] = &m_buffer;

    m_descriptor.get(descriptorInfo);
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
