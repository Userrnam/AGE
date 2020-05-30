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

class TileMap : public Drawable, public Transformable {
    std::vector<Tile> m_tiles;
    MapData m_mapData;
    Buffer m_buffer;
    uint32_t m_maxSize = 0;
protected:
public:
    void create(Layer* layer, Texture& texture, uint32_t maxSize, bool blendEnable);

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
