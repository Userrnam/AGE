#pragma once

#include "Graphics/Viewport.hpp"
#include "Graphics/Camera.hpp"

// #include <vulkan/vulkan.h>

namespace age {

class View {
    Viewport m_viewport;
    Camera camera;
    
    friend class Drawable;
    friend class Scene;
protected:
    const Viewport& getViewport() const { return m_viewport; }
    void clearWindow(int i, const glm::vec4 clearColor = {});
public:

    void init(const Viewport& viewport = {});
    void destroy();
};

} // namespace age
