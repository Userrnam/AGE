#pragma once

#include <vector>
#include <sstream>
#include <glm/glm.hpp>

#include "ShaderComponent.hpp"
#include "ArrayComponent.hpp"

#include "MemoryHolders/Buffer.hpp"

namespace age {

// text should use this
class TileIndexer {
    glm::vec2 m_position = {};
    uint32_t m_index = 0;
    float dummy;
public:
    inline TileIndexer& setPosition(glm::vec2 position) { m_position = position; return *this; }
    inline TileIndexer& setIndex(uint32_t index) { m_index = index; return *this; }
    inline glm::vec2 getPosition() { return m_position; }
    inline uint32_t getIndex() { return m_index; }

    static ShaderComponentInfo __getInfo() {
        ShaderComponentInfo info;
        info.add(
            ShaderComponentBuffer()
            .addBlockMember("vec2 position")
            .addBlockMember("uint index")
            .addBlockMember("float dummy")
            .setVertMainInsert(
                "\ttransform *= mat4(\n"
                "\tvec4(1.0, 0.0, 0.0, 0.0),\n"
                "\tvec4(0.0, 1.0, 0.0, 0.0),\n"
                "\tvec4(0.0, 0.0, 1.0, 0.0),\n"
                "\tvec4(?.position.x, ?.position.y, 0.0, 1.0)\n"
                "\t);\n"
                "\tuint tileIndex = ?.index;\n"
            )
        );

        return info;
    }
};

struct __Tile {
    glm::vec2 texCoords[4];
    glm::vec2 size;
    glm::vec2 dummy;

    static ShaderComponentInfo __getInfo() {
        ShaderComponentInfo info;
        info.add(
            ShaderComponentBuffer()
            .addBlockMember("vec2 texCoords[4]")
            .addBlockMember("vec2 size")
            .addBlockMember("vec2 dummy")
            .setVertMainInsert(
                "\ttransform *= mat4(\n"
                "\tvec4(?.size.x, 0.0, 0.0, 0.0),\n"
                "\tvec4(0.0, ?.size.y, 0.0, 0.0),\n"
                "\tvec4(0.0, 0.0, 1.0, 0.0),\n"
                "\tvec4(0, 0, 0.0, 1.0)\n"
                "\t);\n"
                "\tglobals.texCoords = ?.texCoords[gl_VertexIndex];\n\n"
            )
        );
        info.add(
            ShaderComponentForward("vec2 texCoords")
        );
        return info;
    }
};

const char TILE_INDEX[] = "[tileIndex]";

typedef ArrayComponent<__Tile, TILE_INDEX> TileMapComponent;

} // namespace age
