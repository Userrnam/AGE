#pragma once

#include "Core/Buffer.hpp"
#include "Viewport.hpp"
#include "Camera.hpp"

namespace age {

class View {
    Camera m_camera;
    Viewport m_viewport;
public:
    View();

    void move(const glm::vec2& direction) { m_camera.move(direction); }
    void setScale(const glm::vec2& scale) { m_camera.setScale(scale); }
    void setRotation(float rotation) { m_camera.setRotation(rotation); }

    void move(float x, float y) { m_camera.move(x, y); }
    void setScale(float scaleX, float scaleY) { m_camera.setScale(scaleX, scaleY); }

    void upload() { m_camera.upload(); }

    Camera& getCamera() { return m_camera; }
    Viewport getViewport() { return m_viewport; }
};

} // namespace age
