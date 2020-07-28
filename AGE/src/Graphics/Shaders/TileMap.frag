#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 texCoord;
layout(location = 1) in vec4 color;

layout(set = 1, binding = 1) uniform sampler2D texSampler;

layout(location = 0) out vec4 outColor;

void main() {
    vec4 sampledColor = texture(texSampler, texCoord);
    if (sampledColor == vec4(0.0)) {
        discard;
    }

    outColor = color * sampledColor;
}