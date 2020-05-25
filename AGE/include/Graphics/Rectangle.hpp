#pragma once

#include <glm/glm.hpp>

#include "Drawable.hpp"
#include "Buffer.hpp"
#include "Transformable.hpp"
// #include "View.hpp"
#include "Layer.hpp"
#include "Texture.hpp"

namespace age {

struct TexturedRectangleUniform {
    glm::mat4 transform;
    glm::vec2 texCoords[4];
};

class TexturedRectangleInstance;
class TexturedRectangleFactory : public Drawable {
    std::vector<TexturedRectangleUniform> m_ubos;
    uint32_t m_totalSize = 0; // size of ubos in bytes
    Buffer m_uboBuffer; // sizeof(uniform) * count
    uint32_t m_count = 0;     // max rectangle count
public:
    void destroy();
    void create(Layer* layer, uint32_t count, Texture& texture, bool colorBlending = false);
    void addChild(TexturedRectangleInstance& instance);
    void upload();
};

class TexturedRectangleInstance : public Transformable {
    TexturedRectangleUniform* m_uniform = nullptr;
    Buffer* m_uboBuffer = nullptr;
    uint32_t m_factoryOffset = 0;
    friend class TexturedRectangleFactory;
public:
    void setTexCoords(glm::vec2 coords[4]);

    void updateTransform();
    void upload();
};

} // namespace age
