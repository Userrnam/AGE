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
    virtual std::string getVertStartInsert(int binding, int& outLocation) override {
        std::stringstream ss;
        ss << "layout(set=1, binding=" << binding <<") uniform TexCoordObject {\n";
        ss << "\tvec2 texCoords;\n";
        ss << "} texCoordObject;\n";

        ss << "layout(location=" << outLocation << ") out vec2 texCoords;\n";
        outLocation++;

        return ss.str();
    }
    
    virtual std::string getVertEndInsert() override {
        return "texCoords = texCoordObject.texCoords;\n";
    }
    
    virtual std::string getFragStartInsert(int binding, int& inLocation) override {
        std::stringstream ss;
        ss << "layout(location=" << inLocation << ") in vec2 texCoords;\n";
        inLocation++;

        return ss.str();
    }
    
    virtual std::string getFragEndInsert() override {
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
    virtual std::string getVertStartInsert(int binding, int& outLocation) override {
        return "";
    }
    
    virtual std::string getVertEndInsert() override {
        return "";
    }
    
    virtual std::string getFragStartInsert(int binding, int& inLocation) override {
        std::stringstream ss;
        ss << "layout(set=1, binding=" << binding << ") uniform sampler2D textureSampler;\n";

        return ss.str();
    }
    
    virtual std::string getFragEndInsert() override {
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
