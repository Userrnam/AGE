#pragma once

#include "Camera.hpp"
#include "../ObjectCreation/Descriptor.hpp"
#include "../MemoryHolders/Buffer.hpp"
#include "../UnmanagedTransformable.hpp"

namespace age {

typedef Vector2i(*ResizeHandler)(Vector2i oldSize, Vector2i newSize);

Vector2i maxSizeResizeHandler(Vector2i oldSize, Vector2i newSize);
Vector2i preserveWidthResizeHandler(Vector2i oldSize, Vector2i newSize);
Vector2i preserveHeighthResizeHandler(Vector2i oldSize, Vector2i newSize);


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
    ResizeHandler m_resizeHandler = maxSizeResizeHandler;
public:
    void create();
    void destroy();

    inline DescriptorSet getDescriptor() const { return m_descriptor; }

    inline void setResizeHandler(ResizeHandler handler) { m_resizeHandler = handler; }
    inline ResizeHandler getResizeHandler() const { return m_resizeHandler; }

    void updateCameraTransform();
    void handleWindowResize(const Vector2i& oldSize, const Vector2i newSize);
    void update(float elapsedTime, float currentTime);
};

} // namespace age
