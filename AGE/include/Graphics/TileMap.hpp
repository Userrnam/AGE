#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Buffer.hpp"
#include "Texture.hpp"
#include "Transformable.hpp"
#include "Drawable.hpp"

namespace age {

struct Tile {
    glm::vec2 size;
    glm::vec2 position;
    glm::vec2 bottomLeftTexCoord;
    glm::vec2 topRightTexCoord;
};

struct MapData {
    glm::mat4 transform;
    glm::vec4 color;
};

template<typename T>
struct InstancedComponent {
    std::vector<T> m_instances;
    Buffer buffer;
    uint32_t maxSize = 0;

    void upload() {
        buffer.load(m_instances.data(), m_instances.size());
    }

    inline void addInstance(T instance) {
        m_instances.push_back(instance);
    }
};

/*

tilemap = registry.create();
registry.emplace<ColorComponent>(tilemap);
registry.emplace<TextureComponent>(tilemap);
registry.emplace<TransformComponent>(tilemap);
registry.emplace<InstancedComponent<Tile>>(tilemap);
Rectangle.create based on components
registry.emplace<Drawable>(tilemap, Rectangle);

Drawable: descriptorSet, shader

for shader
generate shader automatically
each component can have some virtual function that will tell what to insert in shader

// insert in global scope
void getVertStartInsert() {
    // Example:
    return "layout(set=1, binding = ..) uniform ColorObject {vec4 color;} colorObject; layout(location=..) out vec4 color;"
}

// insert in main
void getVertEndInsert() {
    // Example:
    return "color = colorObject.color;"
}

// same for frag

for descriptor set also some virtual function but its different for buffer and Texture
stage, type, buffer/texture
getDescription() {
    Description.type = ...
    stage 
    variant<Texture/buffer> 
}

getDescriptorSet(std::vector<Component*>) {
    DescriptorSetInfo info;
    for c in components {
        data = c->getDescription();
        info.set()
        ...
    }
    return DescriptorSet.get();
}

*/

class TileMap : public Drawable, public Transformable {
    std::vector<Tile> m_tiles;          // this is for multiple Instances
    Buffer m_buffer;                    // this is for multiple Instances
    uint32_t m_maxSize = 0;             // this is for multiple Instances

    Shared<ShapeInfo> m_shapeInfo;      // this is temporary
    MapData m_mapData;                  // this is extra data (transform, color) maybe divide it in transform + color components
public:
    void create(View& view, Texture& texture, uint32_t maxSize, bool blendEnable);

    void addTile(const Tile& tile);
    void removeTile(size_t index);
    void clearTiles();
    size_t getTilesCount() const { return m_tiles.size(); }

    void uploadMapData();
    void uploadTiles();
    void setColor(const glm::vec4& color);
    void setColor(float r, float g, float b, float a);
    void destroy();
};

} // namespace age
