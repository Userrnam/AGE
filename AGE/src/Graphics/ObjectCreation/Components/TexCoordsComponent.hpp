#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <sstream>

#include "ShaderComponent.hpp"

namespace age {

struct TexCoords {
    Vector4f coords[4] = {
        // only first two values of vec4 are used (vec4 required for alignment)
        {0, 1, 0, 0}, {1, 1, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}
    };

    static ShaderComponentInfo __getInfo() {
        ShaderComponentInfo info;
        info.add(
            ShaderComponentBuffer()
            .addBlockMember("vec4 texCoords[4]")
            .setVertMainInsert("\tglobals.texCoords = ?.texCoords[gl_VertexIndex].xy;\n")
        );
        info.add(
            ShaderComponentForward("vec2 texCoords")
        );
        return info;
    }
};

} // namespace age
