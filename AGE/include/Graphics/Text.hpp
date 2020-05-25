#pragma once

#include <string>
#include <vector>

#include "Font.hpp"
#include "Rectangle.hpp"
#include "Transformable.hpp"
#include "Layer.hpp"

namespace age {

class Text : public Transformable {
    Font* m_font;
    TexturedRectangleFactory m_charBoxes;
    std::vector<TexturedRectangleInstance> m_trInstances;
public:
    void create(Layer* layer, Font& font, uint32_t maxSize = 128);
    void setText(const std::string& text);
    void draw(int i);
    void destroy();
};

} // namespace age
