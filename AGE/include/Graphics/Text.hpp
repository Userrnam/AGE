#pragma once

#include <string>
#include <vector>

#include "Font.hpp"
#include "Transformable.hpp"
#include "View.hpp"
#include "TileMap.hpp"

namespace age {

class Text : public TileMap {
    Font* m_font;
public:
    void create(View& view, Font& font, uint32_t maxSize = 128);
    void setText(const std::string& text);
};

} // namespace age
