#pragma once

#include "Camera.hpp"
#include "../ObjectCreation/Descriptor.hpp"
#include "../MemoryHolders/Buffer.hpp"
#include "../UnmanagedTransformable.hpp"
#include "../ObjectCreation/Components/ShaderComponent.hpp"

namespace age {

typedef Vector2i(*ResizeHandler)(Vector2i oldSize, Vector2i newSize);

Vector2i maxSizeResizeHandler(Vector2i oldSize, Vector2i newSize);
Vector2i preserveWidthResizeHandler(Vector2i oldSize, Vector2i newSize);
Vector2i preserveHeighthResizeHandler(Vector2i oldSize, Vector2i newSize);


class View : public UnmanagedTransformable {
    float m_zPos = 0.0f;

    struct ViewGlobals {
        glm::mat4 cameraTransform;
        Vector2f resolution;
        float time;
        float deltaTime;
    };

    ViewGlobals m_globals;

    Buffer m_buffer;
    // DescriptorSet m_descriptor;

    Camera m_camera;
    ResizeHandler m_resizeHandler = maxSizeResizeHandler;
public:
    void create(float zPos);
    void destroy();

    float getZ() { return m_zPos; }

    // inline DescriptorSet getDescriptor() const { return m_descriptor; }

    inline void setResizeHandler(ResizeHandler handler) { m_resizeHandler = handler; }
    inline ResizeHandler getResizeHandler() const { return m_resizeHandler; }

    void updateCameraTransform();
    void handleWindowResize(const Vector2i& oldSize, const Vector2i newSize);
    void update(float elapsedTime, float currentTime);

    ShaderComponentInfo getInfo() {
        ShaderComponentInfo info;

        info.add(
            ShaderComponentBuffer()
            .addBlockMember("mat4 transform")
            .addBlockMember("vec2 resolution")
            .addBlockMember("float time")
            .addBlockMember("float deltaTime")
            .setVertMainInsert(
                "\tglobals.resolution = ?.resolution;\n"
                "\tglobals.time = ?.time;\n"
                "\tglobals.deltaTime = ?.deltaTime;\n"

                "\ttransform *= ?.transform;\n"
            )
        );

        info.add(ShaderComponentForward("vec2 resolution"));
        info.add(ShaderComponentForward("float time"));
        info.add(ShaderComponentForward("float deltaTime"));

        info.setBuffer(m_buffer);
        info.setId<ViewGlobals>();

        return info;
    }
};

} // namespace age
