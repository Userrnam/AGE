#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <sstream>

#include "ShaderComponent.hpp"
#include "StorageComponent.hpp"

namespace age {

class TexCoordsComponent : public StorageComponent<glm::vec2[4]> {
public:
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

    ShaderComponentInfo getInfo() {
        auto info = __getInfo();
        info.setBuffer(getBuffer());
        return info;
    }
};

} // namespace age
