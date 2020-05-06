#pragma once

#include "Core/Buffer.hpp"
#include "Viewport.hpp"
#include "Camera.hpp"

namespace age {

class Rectangle;

class View {
    Camera camera;
    Viewport viewport;

    friend Camera;
    friend Rectangle; // FIXME
public:
    View();

    void move(const glm::vec2& direction) { camera.move(direction); }
    void setScale(const glm::vec2& scale) { camera.setScale(scale); }
    void setRotation(float rotation) { camera.setRotation(rotation); }

    void move(float x, float y) { camera.move(x, y); }
    void setScale(float scaleX, float scaleY) { camera.setScale(scaleX, scaleY); }

    void upload() { camera.upload(); }

    Camera& getCamera() { return camera; }
};

} // namespace age
