#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (constant_id = 0) const uint MAX_SAMPLE = 1;

struct UniformBufferObject {
    mat4 transform;
    // vec2 texCoords[4];
    vec2 c1;
    vec2 c2;
    vec2 c3;
    vec2 c4;
};

layout(set = 0, binding = 0) uniform CameraObject {
    mat4 transform;
} camera;

layout(set = 1, binding = 0) uniform UboBlock {
    UniformBufferObject ubos[MAX_SAMPLE];
} uboBlock;

layout(location = 0) in vec2 inPosition;
layout(location = 0) out vec2 texCoord;

void main() {
    gl_Position = camera.transform * uboBlock.ubos[gl_InstanceIndex].transform * vec4(inPosition, -1.0, 1.0);
    // texCoord = uboBlock.ubos[gl_InstanceIndex].texCoords[gl_VertexIndex];

    if (gl_VertexIndex == 0) {
        texCoord = uboBlock.ubos[gl_InstanceIndex].c1;
    }
    if (gl_VertexIndex == 1) {
        texCoord = uboBlock.ubos[gl_InstanceIndex].c2;
    }
    if (gl_VertexIndex == 2) {
        texCoord = uboBlock.ubos[gl_InstanceIndex].c3;
    }
    if (gl_VertexIndex == 3) {
        texCoord = uboBlock.ubos[gl_InstanceIndex].c4;
    }
}
