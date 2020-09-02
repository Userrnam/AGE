#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Camera.hpp"
#include "Viewport.hpp"

namespace age {

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

} // namespace age
