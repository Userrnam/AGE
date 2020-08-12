#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 fragColor;
layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform CameraObject {
    mat4 cameraTransform;
    vec2 resolution;
    float time;
    float deltaTime;
} globals;

layout(set = 1, binding = 0) uniform Block {
    vec4 blendColor;
} block;

void main() {
    vec2 resolution = globals.resolution;
    vec2 coord = 20.0 * (gl_FragCoord.xy - resolution) / min(resolution.x, resolution.y);

    float len;

    for (int i = 0; i < 12; ++i) {
        len = length(vec2(coord.x, coord.y));

        coord.x = coord.x - cos(coord.y + sin(len)) + cos(globals.time.x / 9.0);
        coord.y = coord.y - cos(coord.x + sin(len)) + cos(globals.time.x / 12.0);
    }

    outColor = vec4(cos(len * 2.0), cos(len * 3.0), cos(len * 1.0), 1.0);
}
