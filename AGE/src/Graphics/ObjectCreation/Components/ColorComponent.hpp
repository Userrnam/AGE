#pragma once

#include <sstream>
#include <glm/glm.hpp>

#include "ShaderComponent.hpp"
#include "GetSet.hpp"

namespace age {

class Color : public util::GetSet<glm::vec4> {
public:
    static ShaderComponentInfo __getInfo() {
        ShaderComponentInfo info;
        info.add(
            ShaderComponentBuffer()
            .addBlockMember("vec4 color", true)
            .setFragMainInsert("\tfragColor *= globals.color;\n")
        );
        return info;
    }

    inline Color(glm::vec4 v = glm::vec4(1)) { set(v); }
};

} // namespace age
