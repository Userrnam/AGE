#pragma once

#include "Camera.hpp"
#include "../ObjectCreation/Descriptor.hpp"
#include "../MemoryHolders/Buffer.hpp"
#include "../Transformable.hpp"
#include "../PositionManager.hpp"

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

    PositionManager* m_positionManager = defaultPositionManager;

    friend class ViewManager;
public:
    inline uint64_t getId() const { return m_id; }

    inline DescriptorSet getDescriptor() const { return m_descriptor; }

    void updateCameraTransform();

    PositionManager* getPositionManager() { return m_positionManager; }

    void create(PositionManager* p = defaultPositionManager);
    void destroy();
};

} // namespace age
