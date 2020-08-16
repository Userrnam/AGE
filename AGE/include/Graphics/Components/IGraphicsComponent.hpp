#pragma once

#include <variant>
#include <string>
#include <vulkan/vulkan.h>

#include "../Buffer.hpp"
#include "../Texture.hpp"

namespace age {

struct GraphicsComponentDescription {
    VkShaderStageFlags m_stage;
    VkDescriptorType m_type;
    std::variant<Buffer*, Texture*> m_descriptor;

    // we dont need this all buffers go to vertex shader, all textures to fragment
    inline GraphicsComponentDescription& setStage(VkShaderStageFlags stage) {
        m_stage = stage;
        return *this;
    }

    inline GraphicsComponentDescription& setType(VkDescriptorType type) {
        m_type = type;
        return *this;
    }

    inline GraphicsComponentDescription& setBuffer(Buffer& buffer) {
        m_descriptor = &buffer;
        return *this;
    }

    inline GraphicsComponentDescription& setTexture(Texture& texture) {
        m_descriptor = &texture;
        return *this;
    }
};

enum LayoutType {
    UNDEFINED,
    LOCATION,
    BUFFER,
    SAMPLER
};

struct Layout {
    std::string m_name;
    std::string m_typeName;
    std::vector<std::string> m_members;
    LayoutType m_type = LayoutType::UNDEFINED;

    inline Layout& setType(const std::string& typeName, LayoutType type) {
        m_typeName = typeName;
        m_type = type;
        return *this;
    }

    inline Layout& addBlockMember(const std::string& blockMember) {
        m_members.push_back(blockMember);
        return *this;
    }

    inline Layout& setName(const std::string& name) {
        m_name = name;
        return *this;
    }
};

// this is used to generate shaders and descriptors
struct IGraphicsComponent {
    // used to generate vertex shader
    virtual std::vector<Layout> getVertLayouts() {
        std::vector<Layout> layouts;
        return layouts;
    }

    virtual std::string getVertMainInsert() {
        return "";
    }

    virtual std::string getVertRawInsert() {
        return "";
    }

    // used to generate fragment shader
    virtual std::vector<Layout> getFragLayouts() {
        std::vector<Layout> layouts;
        return layouts;
    }

    virtual std::string getFragRawInsert() {
        return "";
    }

    virtual std::string getFragMainInsert() {
        return "";
    }

    // used in descriptorSet
    virtual GraphicsComponentDescription getDescription() = 0;
};

/*
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
        return layouts;
    }

    virtual std::string getFragMainInsert(const std::string& structName) override {
        std::stringstream ss;
        return ss.str();
    }

    virtual GraphicsComponentDescription getDescription() override {

    }
*/

} // namespace age
