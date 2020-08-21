#pragma once

#include <variant>
#include <string>
#include <vector>
#include <vulkan/vulkan.h>

#include "../Buffer.hpp"
#include "../Texture.hpp"

namespace age {

struct ShaderComponentDescription {
    VkShaderStageFlags m_stage;
    VkDescriptorType m_type;
    std::variant<Buffer*, Texture*> m_descriptor;

    // we dont need this all buffers go to vertex shader, all textures to fragment
    inline ShaderComponentDescription& setStage(VkShaderStageFlags stage) {
        m_stage = stage;
        return *this;
    }

    inline ShaderComponentDescription& setType(VkDescriptorType type) {
        m_type = type;
        return *this;
    }

    inline ShaderComponentDescription& setBuffer(Buffer& buffer) {
        m_descriptor = &buffer;
        return *this;
    }

    inline ShaderComponentDescription& setTexture(Texture& texture) {
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

struct BlockMember {
    std::string m_member;
    bool m_forward;

    inline BlockMember(std::string member, bool forward = false)
        : m_member(member), m_forward(forward) {} 
};

struct Layout {
    std::string m_name;
    std::string m_typeName;
    std::vector<BlockMember> m_members;
    LayoutType m_type = LayoutType::UNDEFINED;
    // this is needed only if layout is bufferblock
    bool m_instanced = false;

    inline Layout& setType(const std::string& typeName, LayoutType type) {
        m_typeName = typeName;
        m_type = type;
        return *this;
    }

    inline Layout& addBlockMember(const std::string& blockMember, bool forward = false) {
        m_members.push_back( { blockMember, forward } );
        return *this;
    }

    inline Layout& setName(const std::string& name) {
        m_name = name;
        return *this;
    }
};

struct TempTexture {
    void setName();
};
struct TempBuffer {
    void setInstanced();
    void addMember();
};
struct TempForward {};

struct Temp {
    std::vector<std::variant<TempTexture, TempBuffer, TempForward>> m_data;
    std::string rawInsert;
    std::string mainInsert;
};

struct Layouts {
    std::vector<Layout> m_layouts;

    inline Layouts& addLayout(Layout layout) {
        m_layouts.push_back(layout);
        return *this;
    }
};

struct ShaderComponentInsert {
    std::vector<Layout> m_layouts;
    std::string m_rawInsert;
    std::string m_mainInsert;

    inline ShaderComponentInsert& addLayout(Layout layout) {
        m_layouts.push_back(layout);
        return *this;
    }

    inline ShaderComponentInsert& setLayouts(Layouts _layouts) {
        m_layouts = _layouts.m_layouts;
        return *this;
    }

    inline ShaderComponentInsert& setRawInsert(std::string s) {
        m_rawInsert = s;
        return *this;
    }

    inline ShaderComponentInsert& setMainInsert(std::string s) {
        m_mainInsert = s;
        return *this;
    }
};

struct ShaderComponentInfo {
    ShaderComponentInsert m_vert;
    ShaderComponentInsert m_frag;
    ShaderComponentDescription m_description;
    bool m_instanced = false;

    inline ShaderComponentInfo& setVertInsert(ShaderComponentInsert vert) {
        m_vert = vert;
        return *this;
    }

    inline ShaderComponentInfo& setFragInsert(ShaderComponentInsert frag) {
        m_frag = frag;
        return *this;
    }

    inline ShaderComponentInfo& setDescription(ShaderComponentDescription description) {
        m_description = description;
        return *this;
    }

    inline ShaderComponentInfo& setInstanced(bool instanced) {
        m_instanced = instanced;
        return *this;
    }
};

} // namespace age
