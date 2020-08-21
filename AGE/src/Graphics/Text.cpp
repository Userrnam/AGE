#include "Text.hpp"
#include <iostream>

namespace age {

void Text::create(View& view, Font& font, uint32_t maxSize) {
    m_font = &font;
    TileMap::create(view, font.m_atlas, maxSize, true);
}

void Text::setText(const std::string& text) {
    TileMap::clearTiles();
    int move = 0;
    for (char c : text) {
        auto& character = m_font->m_characters[c];

        Tile tile;
        memcpy(tile.texCoords, character.texCoords, sizeof(glm::vec2) * 4);
        tile.position = { (move + character.bearing.x), (character.bearing.y - character.size.y) };
        tile.size = character.size;

        TileMap::addTile(tile);

        move += character.advance;
    }

    TileMap::uploadTiles();
}

} // namespace age
