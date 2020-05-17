#pragma once

#include <glm/glm.hpp>

#include "Object.hpp"
#include "Core/Buffer.hpp"
#include "Transformable.hpp"
#include "View.hpp"
#include "Texture.hpp"

namespace age {

struct RectangleUniform {
    glm::mat4 transform;
    glm::vec4 color;
};

struct TexturedRectangleUniform {
    glm::mat4 transform;
    glm::vec2 texCoords[4];
};

class Rectangle : public Object, public Transformable {
    core::Buffer m_uboBuffer;
    glm::vec4 m_color = {};
    bool m_isOwner;

    void preCreate(View& view, ObjectCreateInfo& objectCreateInfo);
public:
    void destroy();

    void create(const Rectangle& sample);
    void create(const Rectangle& sample, Texture& texture);
    void create(View& view, bool colorBlending = false);
    void create(View& view, Texture& texture, bool colorBlending = false);

    glm::vec4 getColor() const { return m_color; }
    void setColor(const glm::vec4& color);
    void setColor(float r, float g, float b, float a);

    void uploadUniform(const RectangleUniform& uniform);
    void upload();
};

// Factory<Rectangle> rFactory;
// Instance<Rectangle> fI = rFactory.getInstance();

class RectangleInstance;
class TexturedRectangleInstance;

// factory
// FIXME:
class RectangleFactory : public Object {
    std::vector<RectangleUniform> m_ubos;
    uint32_t m_totalSize = 0; // size of ubos in bytes
    core::Buffer m_uboBuffer; // sizeof(uniform) * count
    uint32_t m_count = 0;   // max rectangle count
public:
    void destroy();
    void create(View& view, uint32_t count, bool colorBlending = false);
    void addChild(RectangleInstance& instance);
    void upload();
};

class TexturedRectangleFactory : public Object {
    std::vector<TexturedRectangleUniform> m_ubos;
    uint32_t m_totalSize = 0; // size of ubos in bytes
    core::Buffer m_uboBuffer; // sizeof(uniform) * count
    uint32_t m_count = 0;     // max rectangle count
public:
    void destroy();
    void create(View& view, uint32_t count, Texture& texture, bool colorBlending = false);
    void addChild(TexturedRectangleInstance& instance);
    void upload();
};

class RectangleInstance : public Transformable {
    RectangleUniform* m_uniform = nullptr;
    core::Buffer* m_uboBuffer = nullptr;
    uint32_t m_factoryOffset = 0;
    friend class RectangleFactory;
public:
    glm::vec4 getColor() const { return m_uniform->color; }
    void setColor(const glm::vec4& color);
    void setColor(float r, float g, float b, float a);

    void updateTransform();
    void upload();
};

class TexturedRectangleInstance : public Transformable {
    TexturedRectangleUniform* m_uniform = nullptr;
    core::Buffer* m_uboBuffer = nullptr;
    uint32_t m_factoryOffset = 0;
    friend class TexturedRectangleFactory;
public:
    void setTexCoords(glm::vec2 coords[4]);

    void updateTransform();
    void upload();
};

} // namespace age
