#include <glm/gtc/matrix_transform.hpp>

#include "Transformable.hpp"


namespace age {

glm::mat4 Transformable::getTransform() {
    auto originMat = glm::translate(glm::mat4(1.0f), glm::vec3(m_origin.x, m_origin.y, 0));
    auto translateMat = glm::translate(glm::mat4(1.0f), glm::vec3(m_pos.x, m_pos.y, 0));
    auto rotateMat = glm::rotate(glm::mat4(1.0f), m_rotation, glm::vec3(0, 0, 1));
    auto scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(m_scale.x, m_scale.y, 1));

    return translateMat * rotateMat * originMat * scaleMat;
}

} // namespace age
