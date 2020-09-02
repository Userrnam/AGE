#pragma once

#include <vector>
#include <sstream>
#include <glm/glm.hpp>

#include "ShaderComponent.hpp"
#include "ArrayComponent.hpp"

#include "../../MemoryHolders/Buffer.hpp"

namespace age {

// Fixme
struct __Tile {
    glm::vec2 size;
    glm::vec2 position;
    glm::vec2 texCoords[4];

    static ShaderComponentInfo __getInfo() {
        ShaderComponentInfo info;
        info.add(
            ShaderComponentBuffer()
            .addBlockMember("vec2 size")
            .addBlockMember("vec2 position")
            .addBlockMember("vec2 texCoords[4]")
        );
        info.add(
            ShaderComponentForward("vec2 texCoords")
        );
        info.setVertMainInsert(
            "\ttransform *= vec4(?.size.x, 0.0, 0.0, 0.0),\n"
            "\tvec4(0.0, ?.size.y, 0.0, 0.0),\n"
            "\tvec4(0.0, 0.0, 1.0, 0.0),\n"
            "\tvec4(?.position.x, ?.position.y, 0.0, 1.0);\n"
            "\tglobals.texCoords = ?.texCoords[gl_VertexIndex];\n\n"
        );
        return info;
    }
};

typedef ArrayComponent<__Tile, PER_INSTANCE> TileMapComponent;

} // namespace age
