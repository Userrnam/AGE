#include "Text.hpp"


namespace age {

void Text::create(View& view, Font& font, uint32_t maxSize) {
    m_font = &font;
    m_charBoxes.create(view, maxSize, font.m_atlas, true);
}

void Text::setText(const std::string& text) {
    uint32_t move = 0;
    for (char c : text) {
        auto& character = m_font->m_characters[c];

        TexturedRectangleInstance trInstance;
        m_charBoxes.addChild(trInstance);
        trInstance.setPosition(move, 0);
        trInstance.setScale(character.size);
        trInstance.move(character.bearing.x, character.size.y - character.bearing.y);
        trInstance.setTexCoords(character.texCoords);
        trInstance.updateTransform();
        m_trInstances.push_back(trInstance);

        move += character.advance;
    }
    m_charBoxes.upload();
}

void Text::draw(int i) {
    m_charBoxes.draw(i);
}

void Text::destroy() {
    m_charBoxes.destroy();
}

} // namespace age
