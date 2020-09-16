#pragma once

#include "Viewport.hpp"
#include "Camera.hpp"
#include "ObjectCreation/Descriptor.hpp"
#include "MemoryHolders/Buffer.hpp"
#include "Transformable.hpp"
#include <bits/stdint-uintn.h>

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

    uint64_t m_id;

    friend class ViewManager;
public:
    inline uint64_t getId() const { return m_id; }

    inline Viewport getViewport() const { return m_viewport; }
    inline DescriptorSet getDescriptor() const { return m_descriptor; }

    void create(const Viewport& viewport = {});
    void setViewport(const Viewport& viewport = {});
    void destroy();
};

} // namespace age
