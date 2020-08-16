#pragma once

#include <sstream>

#include "../Transformable.hpp"
#include "IGraphicsComponent.hpp"

namespace age {

class TransformComponent : public Transformable, public IGraphicsComponent {
    Buffer buffer;
    uint32_t m_bufferOffset;
public:
    void create() {
        m_bufferOffset = 0;
        buffer.create(
            BufferCreateInfo()
            .setMemoryProperties(VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
            .setSize(sizeof(glm::mat4))
            .setUsage(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
        );
    }

    void upload() {
        buffer.load(&m_transform, sizeof(glm::mat4), m_bufferOffset);
    }

    void destroy() {
        buffer.destroy();
    }

// Interface
    virtual std::vector<Layout> getVertLayouts() override {
        std::vector<Layout> layouts;
        layouts.push_back(
            Layout()
            .setName("transformObject")
            .setType("uniform", LayoutType::BUFFER)
            .addBlockMember("mat4 transform")
        );
        return layouts;
    }

    virtual std::string getVertMainInsert() override {
        return "transform *= ?.transform;\n";
    }

    virtual GraphicsComponentDescription getDescription() override {
        GraphicsComponentDescription description;
        description.m_stage = VK_SHADER_STAGE_VERTEX_BIT;
        description.m_type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        description.m_descriptor = &buffer;

        return description;
    }
};

} // namespace age
    