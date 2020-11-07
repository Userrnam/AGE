#include <glm/gtc/matrix_transform.hpp>

#include "Camera.hpp"

namespace age {

void Camera::setOrthoganalProjection(float width, float height, float zNear, float zFar) {
    m_projection = glm::ortho(
		0.0f, width,
		height, 0.0f,
        zNear, zFar
	);
}

void Camera::setPerspectiveProjection(float width, float height, float angle, float zNear, float zFar) {
    m_projection = glm::perspective(
        angle, width / height, zNear, zFar
    );
}

} // namespace age
