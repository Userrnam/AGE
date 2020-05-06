#pragma once

#include <glm/glm.hpp>

#include "Core/Buffer.hpp"
#include "Core/Descriptor.hpp"
#include "Transformable.hpp"

namespace age {

class View;
class Rectangle;

class Camera : public Transformable {
    struct {
        VkDescriptorSet set;
        VkDescriptorSetLayout layout;
    } m_descriptor;
    core::Buffer m_buffer;
    glm::mat4 m_projection = glm::mat4(1.0f);
    friend Rectangle;
public:
    Camera();
    ~Camera();

    void setOrthoganalProjection(View& view, float zNear = 0.0f, float zFar = 1.0f);
    void setPerspectiveProjection(View& view, float angle = 3.14/4., float zNear=0.1f, float zFar=10.0f);

    void upload();
};

} // namespace age
