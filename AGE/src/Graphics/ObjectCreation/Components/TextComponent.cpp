#include "TextComponent.hpp"

namespace age {

void TextComponent::create(FontComponent* font, uint32_t maxSize) {
    m_font = font;
    ArrayComponent<TileIndexer, PER_INSTANCE>::create(maxSize);
}

void TextComponent::setText(const std::string& text) {
    using Tiles = ArrayComponent<TileIndexer, PER_INSTANCE>;
    Tiles::clear();

    int move = 0;
    for (char c : text) {
        auto& character = m_font->m_characters[c];

        TileIndexer tile;
        tile.setPosition({ (move + character.bearing.x), (character.bearing.y) });
        tile.setIndex(c - m_font->m_firstCharacter);

        Tiles::add(tile);

        move += character.advance;
    }

    Tiles::upload();
}

} // namespace age
