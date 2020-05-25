#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 fragColor;
layout(location = 0) out vec4 outColor;

layout(set = 1, binding = 0) uniform Block {
    vec4 blendColor;
} block;

void main() {
    outColor = block.blendColor * fragColor;
}
