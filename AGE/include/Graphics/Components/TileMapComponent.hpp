#pragma once

#include <vector>
#include <sstream>
#include <glm/glm.hpp>

#include "IGraphicsComponent.hpp"
#include "../Buffer.hpp"

namespace age {

struct Tile {
    glm::vec2 bottomLeftTexCoord;
    glm::vec2 topRightTexCoord;
};

// how to handle id?

// does not contain actual texture
class TileMapComponent : public IGraphicsComponent {
    std::vector<Tile> m_tiles;
    Buffer m_buffer;
    uint32_t m_bufferOffset;

public:
    void create(uint32_t tileCount) {
        m_bufferOffset = 0;
        
        m_buffer.create(
            BufferCreateInfo()
            .setUsage(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
            .setMemoryProperties(VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
            .setSize(sizeof(Tile) * tileCount)
        );
    }

    virtual std::vector<Layout> getVertLayouts() override {
        std::vector<Layout> layouts;
        layouts.push_back(
            Layout()
            .setName("tilesObject")
            .setType("readonly buffer", LayoutType::BUFFER)
            .addBlockMember("vec2 bottomLeftTexCoord")
            .addBlockMember("vec2 topRightTexCoord")
        );
        layouts.push_back(
            Layout()
            .setName("texCoords")
            .setType("vec2", LayoutType::LOCATION)
        );
        return layouts;
    }

    virtual std::string getVertMainInsert() override {
        std::stringstream ss;

        // vec2 tiles[];
        // texCoords = structName.tiles[id * 4 + gl_InstanceIndex]

        ss << "if (gl_VertexIndex == 0) {\n"
            "texCoords = ?.bottomLeftTexCoord;\n"
            "else if (gl_VertexIndex == 1) {\n"
            "texCoords = vec2(?.topRightTexCoord.x, ?.bottomLeftTexCoord.y);\n"
            "else if (gl_VertexIndex == 2) {\n"
            "texCoords = ?.topRightTexCoord;\n"
            "else {\n"
            "texCoords = vec2(?.bottomLeftTexCoord.x, ?.topRightTexCoord.y);\n";

        return ss.str();
    }

    virtual std::vector<Layout> getFragLayouts() override {
        std::vector<Layout> layouts;
        layouts.push_back(
            Layout()
            .setName("texCoords")
            .setType("vec2", LayoutType::LOCATION)
        );
        return layouts;
    }

    virtual GraphicsComponentDescription getDescription() override {
        GraphicsComponentDescription description;

        return description;
    }
};

} // namespace age
