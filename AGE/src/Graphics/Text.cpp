#include "Text.hpp"
#include <iostream>

namespace age {

void Text::create(Layer* layer, Font& font, uint32_t maxSize) {
    m_font = &font;
    TileMap::create(layer, font.m_atlas, maxSize, true);
}

void Text::setText(const std::string& text) {
    TileMap::clearTiles();
    int move = 0;
    for (char c : text) {
        auto& character = m_font->m_characters[c];

        Tile tile;
        tile.bottomLeftTexCoord = character.bottomLeftTexCoord;
        tile.topRightTexCoord = character.topRightTexCoord;
        tile.position = { (move + character.bearing.x), (character.bearing.y - character.size.y) };
        tile.size = character.size;

        TileMap::addTile(tile);

        move += character.advance;
    }

    TileMap::uploadTiles();
}

} // namespace age
