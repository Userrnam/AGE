#pragma once

#include <unordered_map>

#include "TileMapComponent.hpp"
#include "TextureComponent.hpp"

namespace age {

struct _Character {
    glm::ivec2 bearing;
    unsigned advance;
    unsigned index;
};

struct FontInfo {
    unsigned fontSize = 50;
    std::string loadChars;
    Sampler sampler = Sampler::getDefault();

    FontInfo& setFontSize(unsigned size) { fontSize = size; return *this; }
    FontInfo& setLoadChars(const std::string& chars) { loadChars = chars; return *this; }
    FontInfo& setSampler(const Sampler& _sampler) { sampler = _sampler; return *this; }
};

class FontComponent : public TileMapComponent, public TextureComponent {
    std::unordered_map<char, _Character> m_characters;
    float m_height = 0;
    float m_yShift = 0;

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

    void load(const std::string& fontPath, const FontInfo& info = FontInfo());
    void destroy();
};

}
