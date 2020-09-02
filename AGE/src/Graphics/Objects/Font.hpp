#pragma once

#include <string>
#include <map>
#include <glm/glm.hpp>

#include "Texture.hpp"

namespace age {

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
    void load(const std::string& fontPath, unsigned fontSize = 100, Sampler sampler = Sampler::getDefault());
    void destroy();
};

} // namespace age
