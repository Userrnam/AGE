#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <sstream>

#include "ShaderComponent.hpp"

namespace age {

class TexCoordsComponent {
    std::vector<glm::vec2> m_texCoords;
    Buffer m_buffer;
    uint32_t m_bufferOffset;
public:
    void create(uint32_t coordCount)  {
        m_texCoords.resize(coordCount);
        m_bufferOffset = 0;

        m_buffer.create(
            BufferCreateInfo()
            .setMemoryProperties(VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
            .setUsage(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
            .setSize(coordCount * sizeof(glm::vec2))
        );
    }

    inline void upload() {
        m_buffer.load(m_texCoords.data(), sizeof(glm::vec2) * m_texCoords.size(), m_bufferOffset);
    }

    inline void destroy() {
        m_buffer.destroy();
    }

    ShaderComponentInfo getInfo() {
        ShaderComponentInfo info;
        info.setVertInsert(
            ShaderComponentInsert()
            .addLayout(
                Layout()
                .setName("texCoordObject")
                .setType("uniform", LayoutType::BUFFER)
                .addBlockMember("vec2 texCoords", true)
            )
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
