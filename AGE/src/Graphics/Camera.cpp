#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Camera.hpp"

#include "View.hpp"
#include "Core/DescriptorManager.hpp"

namespace age {

Camera::Camera() {
    core::BufferCreateInfo createInfo;
    createInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
    createInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    createInfo.size = sizeof(glm::mat4);
    m_buffer.create(createInfo);

    // create descriptor
    core::DescriptorInfo descriptorInfo;
    descriptorInfo.ubosBinding = 0;
    descriptorInfo.ubos.resize(1);
    descriptorInfo.ubos[0].buffer = m_buffer.getBuffer();
    descriptorInfo.ubos[0].size = sizeof(glm::mat4);

    auto descriptor = core::getDescriptor(descriptorInfo);
    m_descriptor.set = descriptor.sets[0];
    m_descriptor.layout = descriptor.layouts[0];
}

Camera::~Camera() {
    m_buffer.destroy();
}

void Camera::setOrthoganalProjection(View& view, float zNear, float zFar) {
    m_projection = glm::ortho(
		0.0f, view.viewport.width,
		view.viewport.height, 0.0f,
        zNear, zFar
	);
}

void Camera::setPerspectiveProjection(View& view, float angle, float zNear, float zFar) {
    m_projection = glm::perspective(
        angle, view.viewport.width / view.viewport.height, zNear, zFar
    );
}

void Camera::upload() {
    auto mat = m_projection * getTransform();
    m_buffer.loadData(&mat, sizeof(mat));
}

} // namespace age
