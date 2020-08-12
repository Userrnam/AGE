#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform CameraObject {
    mat4 cameraTransform;
    vec2 resolution;
    float time;
    float deltaTime;
} globals;

layout(location = 0) in vec4 inColor;
layout(location = 1) in vec2 inPosition;
layout(location = 0) out vec4 fragColor;

void main() {
    gl_Position = globals.cameraTransform * vec4(inPosition, -1.0, 1.0);

    fragColor = inColor;
}
