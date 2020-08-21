#version 450
#extension GL_ARB_separate_shader_objects : enable

struct Passing {
    vec2 texCoord;
    vec4 color;
};

layout(location = 0) in Passing passing;

layout(set = 1, binding = 1) uniform sampler2D texSampler;

layout(location = 0) out vec4 outColor;

void main() {
    vec4 sampledColor = texture(texSampler, passing.texCoord);

    outColor = passing.color * sampledColor;
}
