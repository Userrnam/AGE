#pragma once

#include <sstream>
#include <glm/glm.hpp>

#include "IGraphicsComponent.hpp"

namespace age {

class ColorComponent : public IGraphicsComponent {
    glm::vec4 m_color;
    Buffer m_buffer;
public:
    void setColor(const glm::vec4 color) {
        m_color = color;
    }

    glm::vec4 getColor() const {
        return m_color;
    }

    void create() {
        m_buffer.create(
            BufferCreateInfo()
            .setMemoryProperties(VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
            .setSize(sizeof(glm::mat4))
            .setUsage(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
        );
    }

    void upload() {
        m_buffer.load(&m_color, sizeof(glm::vec4));
    }

    void destroy() {
        m_buffer.destroy();
    }

// Interface
    virtual std::string getVertStartInsert(int binding, int& outLocation) override {
        return "";
    }

    virtual std::string getVertEndInsert() override {
        return "";
    }

    virtual std::string getFragStartInsert(int binding, int& inLocation) override {
        std::stringstream ss;
        ss << "layout(set=1, binding=" << binding << ") uniform ColorObject {\n";
        ss << "\tvec4 color;\n";
        ss << "} colorObject;\n";

        return ss.str();
    }

    virtual std::string getFragEndInsert() override {
        return "fragColor *= colorObject.color;\n";
    }

    virtual GraphicsComponentDescription getDescription() override {
        GraphicsComponentDescription description;
        description.stage = VK_SHADER_STAGE_VERTEX_BIT;
        description.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        description.descriptor = &m_buffer;
        
        return description;
    }
};

} // namespace age
