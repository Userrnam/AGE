#pragma once

#include <sstream>
#include <glm/glm.hpp>

#include "IGraphicsComponent.hpp"

namespace age {

class ColorComponent : public IGraphicsComponent {
    glm::vec4 m_color;
    Buffer m_buffer;
    uint32_t m_bufferOffset;
public:
    void setColor(const glm::vec4 color) {
        m_color = color;
    }

    glm::vec4 getColor() const {
        return m_color;
    }

    void create() {
        m_bufferOffset = 0;
        m_buffer.create(
            BufferCreateInfo()
            .setMemoryProperties(VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
            .setSize(sizeof(glm::mat4))
            .setUsage(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
        );
    }

    void upload() {
        m_buffer.load(&m_color, sizeof(glm::vec4), m_bufferOffset);
    }

    void destroy() {
        m_buffer.destroy();
    }

// Interface

    virtual std::vector<Layout> getFragLayouts() override {
        std::vector<Layout> layouts;
        layouts.push_back(
            Layout()
            .setName("colorObject")
            .setType("uniform", LayoutType::BUFFER)
            .addBlockMember(
                "vec4 color"
            )
        );
        return layouts;
    }

    virtual std::string getFragMainInsert() override {
        return "fragColor *= ?.color;\n";
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
