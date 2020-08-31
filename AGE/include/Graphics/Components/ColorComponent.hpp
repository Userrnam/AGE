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
        );
        info.setFragMainInsert("\tfragColor *= globals.color;\n");
        return info;
    }

    Color() {}
    inline Color(glm::vec4 v) { set(v); }
};

} // namespace age
