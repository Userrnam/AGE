#pragma once

#include <vector>
#include <sstream>
#include <glm/glm.hpp>

#include "ShaderComponent.hpp"
#include "InstancedComponent.hpp"

#include "../Buffer.hpp"

namespace age {

// Fixme
struct __Tile {
    glm::vec2 size;
    glm::vec2 position;
    glm::vec2 texCoords[4];
};

class __TileMapComponent {
    __Tile m_data;
    friend class Instanced<__TileMapComponent>;
public:
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

typedef Instanced<__TileMapComponent> TileMapComponent;

} // namespace age
