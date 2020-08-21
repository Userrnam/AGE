#version 450
#extension GL_ARB_separate_shader_objects : enable

struct Tile {
    vec2 size;
    vec2 position;
    vec2 texCoords[4];
};

layout(set = 0, binding = 0) uniform CameraObject {
    mat4 cameraTransform;
    vec2 resolution;
    float time;
    float deltaTime;
} globals;

// maybe load atlas here and use indicies for letters
layout(set = 1, binding = 0) readonly buffer UboBlock {
    mat4 transform;
    vec4 color;
    Tile tiles[];
} uboBlock;

struct Passing {
    vec2 texCoord;
    vec4 color;
};

layout(location = 0) in vec2 inPosition;
layout(location = 0) out Passing passing;

void main() {
    mat4 tileTransform = mat4(
        vec4(uboBlock.tiles[gl_InstanceIndex].size.x, 0.0, 0.0, 0.0),
        vec4(0.0, uboBlock.tiles[gl_InstanceIndex].size.y, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(uboBlock.tiles[gl_InstanceIndex].position.x, uboBlock.tiles[gl_InstanceIndex].position.y, 0.0, 1.0)
    );

    gl_Position = globals.cameraTransform * uboBlock.transform * tileTransform * vec4(inPosition, -1.0, 1.0);

    passing.texCoord = uboBlock.tiles[gl_InstanceIndex].texCoords[gl_VertexIndex];
    passing.color = uboBlock.color;
}
