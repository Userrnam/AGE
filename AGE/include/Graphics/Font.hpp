#pragma once

#include <string>
#include <map>
#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Texture.hpp"

namespace age {

void initFreeType();

struct Character {
    glm::vec2 texCoords[4];
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned advance;
};

class Font {
    std::map<char, Character> m_characters;
    Texture m_atlas;

    friend class Text;
public:
    void load(const std::string& fontPath, unsigned fontSize = 40);
    void destroy();
};

} // namespace age
