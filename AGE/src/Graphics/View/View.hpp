#pragma once

#include "Camera.hpp"
#include "../ObjectCreation/Descriptor.hpp"
#include "../MemoryHolders/Buffer.hpp"
#include "../UnmanagedTransformable.hpp"

namespace age {

class View : public UnmanagedTransformable {
    struct ViewGlobals {
        glm::mat4 cameraTransform;
        Vector2f resolution;
        float time;
        float deltaTime;
    };

    ViewGlobals m_globals;

    Buffer m_buffer;
    DescriptorSet m_descriptor;

    Camera m_camera;

    uint64_t m_id;

    friend class ViewManager;
public:
    inline uint64_t getId() const { return m_id; }

    inline DescriptorSet getDescriptor() const { return m_descriptor; }

    void updateCameraTransform();

    void create();
    void destroy();
};

} // namespace age
