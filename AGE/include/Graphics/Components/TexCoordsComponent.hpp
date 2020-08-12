#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <sstream>

#include "IGraphicsComponent.hpp"

namespace age {

class TexCoordsComponent : public IGraphicsComponent {
    std::vector<glm::vec2> texCoords;
    Buffer buffer;

public:
    void create(uint32_t coordCount)  {
        texCoords.resize(coordCount);

        buffer.create(
            BufferCreateInfo()
            .setMemoryProperties(VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
            .setUsage(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
            .setSize(coordCount * sizeof(glm::vec2))
        );
    }

    void destroy() {
        buffer.destroy();
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

    virtual std::string getVertMainInsert(const std::string& structName) override {
        std::stringstream ss;
        ss << "texCoords = " << structName << ".texCoords;\n";
        return ss.str();
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
        description.m_descriptor = &buffer;

        return description;
    }
};
    
} // namespace age
