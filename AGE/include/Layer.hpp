#pragma once

#include "Graphics/Viewport.hpp"
#include "Graphics/Camera.hpp"

#include <vulkan/vulkan.h>

namespace age {

class Layer {
    Viewport m_viewport;
    bool m_depthTest = false;
    bool m_multisampling = false;
    float m_minSampleShading = 0.2;

    friend class Drawable;
    friend class Application;
protected:
    Camera camera;
    const Viewport& getViewport() const { return m_viewport; }
    void clearWindow(int i, const glm::vec4 clearColor = {});
public:
    Layer(const Viewport& viewport = {}, bool depthTest = false, bool multisampling = false, float minSampleShading = 0.2);
    void destroy();

    virtual void draw(int i);
    virtual void onCreate() {}
    virtual void onDestroy() {}
    virtual void onUpdate(float elapsedTime) {}
};

} // namespace age
