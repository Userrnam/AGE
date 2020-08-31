#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <sstream>

#include "ShaderComponent.hpp"

namespace age {

struct TexCoords {
    glm::vec2 coords[4] = {
        {0, 0}, {1, 0}, {1, 1}, {0, 1}
    };

    static ShaderComponentInfo __getInfo() {
        ShaderComponentInfo info;
        info.add(
            ShaderComponentBuffer()
            .addBlockMember("vec2 texCoords[4]")
        );
        info.add(
            ShaderComponentForward("vec2 texCoords")
        );
        info.setVertMainInsert("\tglobals.texCoords = ?.texCoords[gl_VertexIndex];\n");
        return info;
    }
};

} // namespace age
