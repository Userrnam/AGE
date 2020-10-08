#pragma once

#include <vector>
#include <sstream>
#include "Math.hpp"

#include "ShaderComponent.hpp"
#include "ArrayComponent.hpp"

#include "MemoryHolders/Buffer.hpp"

namespace age {

// text should use this
class TileIndexer {
    Vector2f m_position = {};
    uint32_t m_index = 0;
    float dummy;
public:
    inline TileIndexer& setPosition(Vector2f position) { m_position = position; return *this; }
    inline TileIndexer& setIndex(uint32_t index) { m_index = index; return *this; }
    inline Vector2f getPosition() { return m_position; }
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
    Vector2f texCoords[4];
    Vector2f size;
    Vector2f dummy;

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
