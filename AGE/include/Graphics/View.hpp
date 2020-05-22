#pragma once

#include "Buffer.hpp"
#include "Viewport.hpp"
#include "Camera.hpp"

namespace age {

class View {
    Camera m_camera;
    Viewport m_viewport;
public:
    void create();
    void destroy();

    inline void move(const glm::vec2& direction) { m_camera.move(direction); }
    inline void setScale(const glm::vec2& scale) { m_camera.setScale(scale); }
    inline void setRotation(float rotation) { m_camera.setRotation(rotation); }

    inline void move(float x, float y) { m_camera.move(x, y); }
    inline void setScale(float scaleX, float scaleY) { m_camera.setScale(scaleX, scaleY); }

    inline void upload() { m_camera.upload(); }

    inline Camera& getCamera() { return m_camera; }
    inline const Camera getCamera() const { return m_camera; }

    inline const Viewport& getViewport() const { return m_viewport; }
};

} // namespace age
