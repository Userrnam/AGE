#pragma once

#include <sstream>
#include <glm/glm.hpp>

#include "ShaderComponent.hpp"
#include "GetSet.hpp"

namespace age {

class Transform : public util::GetSet<glm::mat4> {
public:
    static ShaderComponentInfo __getInfo() {
        ShaderComponentInfo info;
        info.add(
            ShaderComponentBuffer()
            .addBlockMember("mat4 transform")
            .setVertMainInsert("\ttransform *= ?.transform;\n")
        );
        return info;
    }

    inline Transform(const glm::mat4& m = glm::mat4(1)) { set(m); }
};

} // namespace age
