#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform CameraObject {
    mat4 transform;
} camera;

layout(set = 1, binding = 0) uniform UniformBufferObject {
    mat4 transform;
    vec4 color;
} ubo;

layout(location = 0) in vec2 inPosition;
layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 texCoord;

void main() {
    gl_Position = camera.transform * ubo.transform * vec4(inPosition, -1.0, 1.0);
    fragColor = ubo.color;
    texCoord = vec2(inPosition.x, 1.0 - inPosition.y);
}
