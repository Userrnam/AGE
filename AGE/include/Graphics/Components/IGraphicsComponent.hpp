#pragma once

#include <variant>
#include <string>
#include <vulkan/vulkan.h>

#include "../Buffer.hpp"
#include "../Texture.hpp"

namespace age {

struct GraphicsComponentDescription {
    VkShaderStageFlags stage;
    VkDescriptorType type;
    std::variant<Buffer*, Texture*> descriptor;
};

enum LayoutType {
    UNDEFINED,
    LOCATION,
    BUFFER,
    SAMPLER
};

struct BlockMember {
    std::string m_name;
    std::string m_type;

    inline BlockMember& setName(const std::string& name) {
        m_name = name;
        return *this;
    }

    inline BlockMember& setType(const std::string& type) {
        m_type = type;
        return *this;
    }
};

struct Layout {
    std::string m_name;
    std::string m_typeName;
    std::vector<BlockMember> m_members;
    LayoutType m_type = LayoutType::UNDEFINED;

    inline Layout& setType(const std::string& typeName, LayoutType type) {
        m_typeName = typeName;
        m_type = type;
        return *this;
    }

    inline Layout& addBlockMember(const BlockMember& blockMember) {
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
    virtual std::vector<Layout> getVertLayouts() = 0;
    virtual std::string getVertMainInsert(const std::string& structName) = 0;

    // used to generate fragment shader
    virtual std::vector<Layout> getFragLayouts() = 0;
    virtual std::string getFragMainInsert(const std::string& structName) = 0;

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

    virtual std::string getVertMainInsert(const std::string& structName) override {
        std::stringstream ss;
        return ss.str();
    }
*/

} // namespace age
