#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <sstream>

#include "IGraphicsComponent.hpp"

namespace age {

class TexCoordsComponent : public IGraphicsComponent {
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

    // Interface
    virtual std::vector<Layout> getVertLayouts() override {
        std::vector<Layout> layouts;
        layouts.push_back(
            Layout()
            .setName("texCoordObject")
            .setType("uniform", LayoutType::BUFFER)
            .addBlockMember("vec2 texCoords")
        );
        layouts.push_back(
            Layout()
            .setName("texCoords")
            .setType("vec2", LayoutType::LOCATION)
        );
        return layouts;
    }

    virtual std::string getVertMainInsert() override {
        return "texCoords = ?.texCoords;\n";
    }

    virtual std::vector<Layout> getFragLayouts() override {
        std::vector<Layout> layouts;
        layouts.push_back(
            Layout()
            .setName("texCoords")
            .setType("vec2", LayoutType::LOCATION)
        );
        return layouts;
    }

    virtual GraphicsComponentDescription getDescription() override {
        GraphicsComponentDescription description;
        description.m_stage = VK_SHADER_STAGE_VERTEX_BIT;
        description.m_type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        description.m_descriptor = &m_buffer;

        return description;
    }
};
    
} // namespace age
