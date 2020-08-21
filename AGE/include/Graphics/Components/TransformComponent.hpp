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
        return info;
    }

    ShaderComponentInfo getInfo() {
        auto info = __getInfo();
        info.setBuffer(getBuffer());
        return info;
    }

    // ShaderComponentInfo getInfo() {
    //     ShaderComponentInfo info;
    //     info.setVertInsert(
    //         ShaderComponentInsert()
    //         .addLayout(
    //             Layout()
    //             .setName("transformObject")
    //             .setType("uniform", LayoutType::BUFFER)
    //             .addBlockMember("mat4 transform")
    //         )
    //         .setMainInsert("\ttransform *= ?.transform\n")
    //     );
    //     info.setDescription(
    //         ShaderComponentDescription()
    //         .setType(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
    //         .setStage(VK_SHADER_STAGE_VERTEX_BIT)
    //         .setBuffer(m_buffer)
    //     );
    //     return info;
    // }
};

} // namespace age
    