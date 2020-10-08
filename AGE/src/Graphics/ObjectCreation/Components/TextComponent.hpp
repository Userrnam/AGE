#pragma once

#include "TileMapComponent.hpp"
#include "FontComponent.hpp"

namespace age {

class TextComponent : public ArrayComponent<TileIndexer, PER_INSTANCE> {
    FontComponent* m_font;
    Vector2f m_size = {};

public:
    ShaderComponentInfo getInfo() {
        auto info = ArrayComponent<TileIndexer, PER_INSTANCE>::getInfo();
        auto fontInfo = m_font->getInfo();
        for (const auto& comp : fontInfo.m_data) {
            info.m_data.push_back(comp);
        }
        
        info.setId<TextComponent>();
        
        return info;
    }

    void create(FontComponent* font, uint32_t maxSize = 128);
    void setText(const std::string& text);
    Vector2f getSize() const { return m_size; }
};

} // namespace age
