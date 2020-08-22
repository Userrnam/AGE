#pragma once

#include <sstream>

#include "ShaderComponent.hpp"
#include "StorageComponent.hpp"

namespace age {

class TransformComponent : public StorageComponent<glm::mat4> {
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

    ShaderComponentInfo getInfo() {
        auto info = __getInfo();
        info.setBuffer(getBuffer());
        info.setId<TransformComponent>();
        return info;
    }
};

} // namespace age
    