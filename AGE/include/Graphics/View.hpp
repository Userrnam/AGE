#pragma once

#include "Viewport.hpp"
#include "Camera.hpp"
#include "Descriptor.hpp"
#include "Buffer.hpp"
#include "Transformable.hpp"

namespace age {

class View : public Transformable {
    struct ViewGlobals {
        glm::mat4 cameraTransform;
        glm::vec2 resolution;
        float time;
        float deltaTime;
    };

    ViewGlobals m_globals;

    Buffer m_buffer;
    DescriptorSet m_descriptor;

    Viewport m_viewport;
    Camera camera;

    friend class ViewManager;
public:

    inline Viewport getViewport() const { return m_viewport; }
    inline DescriptorSet getDescriptor() const { return m_descriptor; }

    void create(const Viewport& viewport = {});
    void destroy();
};

} // namespace age
