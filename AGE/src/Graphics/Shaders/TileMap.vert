#version 450
#extension GL_ARB_separate_shader_objects : enable

struct Tile {
    vec2 size;
    vec2 position;
    vec2 bottomLeftTexCoord;
    vec2 topRightTexCoord;
};

layout(set = 0, binding = 0) uniform CameraObject {
    mat4 transform;
    vec4 time;
} camera;

layout(set = 1, binding = 0) readonly buffer UboBlock {
    mat4 transform;
    vec4 color;
    Tile tiles[];
} uboBlock;

layout(location = 0) in vec2 inPosition;
layout(location = 0) out vec2 texCoord;
layout(location = 1) out vec4 color;

void main() {
    mat4 tileTransform = mat4(
        vec4(uboBlock.tiles[gl_InstanceIndex].size.x, 0.0, 0.0, 0.0),
        vec4(0.0, uboBlock.tiles[gl_InstanceIndex].size.y, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(uboBlock.tiles[gl_InstanceIndex].position.x, uboBlock.tiles[gl_InstanceIndex].position.y, 0.0, 1.0)
    );

    gl_Position = camera.transform * uboBlock.transform * tileTransform * vec4(inPosition, -1.0, 1.0);

    if (gl_VertexIndex == 0) {
        texCoord = uboBlock.tiles[gl_InstanceIndex].bottomLeftTexCoord;
    } else if (gl_VertexIndex == 1) {
        texCoord = vec2(uboBlock.tiles[gl_InstanceIndex].topRightTexCoord.x, uboBlock.tiles[gl_InstanceIndex].bottomLeftTexCoord.y);
    } else if (gl_VertexIndex == 2) {
        texCoord = uboBlock.tiles[gl_InstanceIndex].topRightTexCoord;
    } else if (gl_VertexIndex == 3) {
        texCoord = vec2(uboBlock.tiles[gl_InstanceIndex].bottomLeftTexCoord.x, uboBlock.tiles[gl_InstanceIndex].topRightTexCoord.y);
    }

    color = uboBlock.color;
}
