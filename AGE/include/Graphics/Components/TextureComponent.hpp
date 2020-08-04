#pragma once

#include <sstream>
#include <glm/glm.hpp>
#include <vector>

#include "IGraphicsComponent.hpp"

#include "../Sampler.hpp"
#include "../Texture.hpp"

namespace age {

class TextureCoordComponent : public IGraphicsComponent {
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
            .addBlockMember(
                BlockMember()
                .setName("texCoords")
                .setType("vec2")
            )
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

    virtual std::string getFragMainInsert(const std::string& structName) override {
        return "";
    }

    virtual GraphicsComponentDescription getDescription() override {
        GraphicsComponentDescription description;
        description.stage = VK_SHADER_STAGE_VERTEX_BIT;
        description.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        description.descriptor = &buffer;

        return description;
    }
};

// ! requires TextureCoordComponent
class TextureComponent : public IGraphicsComponent {
    Texture texture;

public:
    void create(std::string path, Shared<Sampler> sampler)  {
        texture.create(path, sampler);
    }

    void destroy() {
        texture.destroy();
    }

    // Interface
    virtual std::vector<Layout> getVertLayouts() override {
        std::vector<Layout> layouts;
        return layouts;
    }

    virtual std::string getVertMainInsert(const std::string& structName) override {
        std::stringstream ss;
        return ss.str();
    }

    virtual std::vector<Layout> getFragLayouts() override {
        std::vector<Layout> layouts;
        layouts.push_back(
            Layout()
            .setName("textureSampler")
            .setType("uniform sampler2D", LayoutType::SAMPLER)
        );
        return layouts;
    }

    virtual std::string getFragMainInsert(const std::string& structName) override {
        return "fragColor *= texture(textureSampler, texCoords);\n";
    }

    virtual GraphicsComponentDescription getDescription() override {
        GraphicsComponentDescription description;
        description.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        description.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        description.descriptor = &texture;

        return description;
    }
};

} // namespace age
