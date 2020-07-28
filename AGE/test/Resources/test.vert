#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform CameraObject {
    mat4 transform;
    vec4 time;
} camera;

layout(location = 0) in vec4 inColor;
layout(location = 1) in vec2 inPosition;
layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec4 time;

void main() {
    gl_Position = camera.transform * vec4(inPosition, -1.0, 1.0);

    time = camera.time;
    fragColor = inColor;
}