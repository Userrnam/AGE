#pragma once

#include "Graphics/Viewport.hpp"
#include "Graphics/Camera.hpp"

#include <vulkan/vulkan.h>

namespace age {

class Layer {
    Viewport m_viewport;
    friend class Drawable;
protected:
    Camera camera;
    const Viewport& getViewport() const { return m_viewport; }
    void clearWindow(int i, const glm::vec4 clearColor = {});
public:
    Layer(const Viewport& viewport = {});
    void destroy();

    virtual void draw(int i);
    virtual void onCreate() {}
    virtual void onDestroy() {}
    virtual void onUpdate(float elapsedTime) {}
};

} // namespace age
