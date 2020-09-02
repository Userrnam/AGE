#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "../MemoryHolders/Buffer.hpp"
#include "Texture.hpp"
#include "../Transformable.hpp"
#include "../ObjectCreation/Drawable.hpp"

namespace age {

struct Tile {
    glm::vec2 size;
    glm::vec2 position;
    glm::vec2 texCoords[4];
};

struct MapData {
    glm::mat4 transform;
    glm::vec4 color;
};

class TileMap : public Drawable, public Transformable {
    std::vector<Tile> m_tiles;          // this is for multiple Instances
    Buffer m_buffer;                    // this is for multiple Instances
    uint32_t m_maxSize = 0;             // this is for multiple Instances

    MapData m_mapData;                  // this is extra data (transform, color) maybe divide it in transform + color components
public:
    void create(Texture& texture, uint32_t maxSize, bool blendEnable);

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
