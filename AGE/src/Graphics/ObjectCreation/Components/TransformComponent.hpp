#pragma once

#include <sstream>

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
        );
        info.setVertMainInsert("\ttransform *= ?.transform;\n");
        return info;
    }

    Transform() {}
    inline Transform(const glm::mat4& m) { set(m); }
};

} // namespace age
