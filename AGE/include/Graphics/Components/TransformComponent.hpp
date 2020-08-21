#pragma once

#include <sstream>

#include "../Transformable.hpp"
#include "ShaderComponent.hpp"

namespace age {

class TransformComponent {
    glm::mat4 m_transform;
    Buffer m_buffer;
    uint32_t m_bufferOffset;
public:
    void create() {
        m_bufferOffset = 0;
        m_buffer.create(
            BufferCreateInfo()
            .setMemoryProperties(VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
            .setSize(sizeof(glm::mat4))
            .setUsage(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
        );
    }

    ShaderComponentInfo getInfo() {
        ShaderComponentInfo info;
        info.setVertInsert(
            ShaderComponentInsert()
            .addLayout(
                Layout()
                .setName("transformObject")
                .setType("uniform", LayoutType::BUFFER)
                .addBlockMember("mat4 transform")
            )
            .setMainInsert("\ttransform *= ?.transform\n")
        );
        info.setDescription(
            ShaderComponentDescription()
            .setType(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
            .setStage(VK_SHADER_STAGE_VERTEX_BIT)
            .setBuffer(m_buffer)
        );
        return info;
    }
};

} // namespace age
    