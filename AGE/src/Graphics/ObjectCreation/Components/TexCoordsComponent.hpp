#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <sstream>

#include "ShaderComponent.hpp"

namespace age {

struct TexCoords {
    glm::vec4 coords[4] = {
        // only first two values of vec4 are used (vec4 required for alignment)
        {0, 1, 0, 0}, {1, 1, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}
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
