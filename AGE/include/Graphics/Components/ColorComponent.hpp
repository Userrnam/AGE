#pragma once

#include <sstream>
#include <glm/glm.hpp>

#include "ShaderComponent.hpp"
#include "StorageComponent.hpp"

namespace age {

class ColorComponent : public StorageComponent<glm::vec4> {
public:
    // this is used by Instanced template
    static ShaderComponentInfo __getInfo() {
        ShaderComponentInfo info;
        info.add(
            ShaderComponentBuffer()
            .addBlockMember("vec4 color", true)
        );
        info.setFragMainInsert("\tfragColor *= globals.color;\n");
        return info;
    }

    ShaderComponentInfo getInfo() {
        ShaderComponentInfo info = __getInfo();
        info.setBuffer(getBuffer());
        return info;
    }
};

} // namespace age
