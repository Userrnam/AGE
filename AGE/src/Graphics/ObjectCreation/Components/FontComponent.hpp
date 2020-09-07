#pragma once

#include <unordered_map>

#include "TileMapComponent.hpp"
#include "TextureComponent.hpp"

namespace age {

struct _Character {
    glm::ivec2 bearing;
    unsigned advance;
};

class FontComponent : public TileMapComponent, public TextureComponent {
    std::unordered_map<char, _Character> m_characters;
    unsigned char m_firstCharacter = 32;

    friend class TextComponent;
public:
    ShaderComponentInfo __getInfo() {
        auto info = TileMapComponent::__getInfo();
        auto textureInfo = TextureComponent::__getInfo();
        info.m_data.push_back(textureInfo.m_data[0]);
        return info;
    }

    ShaderComponentInfo getInfo() {
        auto info = __getInfo();
        // fixme
        std::get<ShaderComponentBuffer>(info.m_data[0]).m_buffer = getBuffer();
        std::get<ShaderComponentTexture>(info.m_data[2]).m_texture = getTexture();

        info.setId<FontComponent>();

        return info;
    }

    void load(const std::string& fontPath, unsigned fontSize = 100, Sampler sampler = Sampler::getDefault());
    void destroy();
};

}
