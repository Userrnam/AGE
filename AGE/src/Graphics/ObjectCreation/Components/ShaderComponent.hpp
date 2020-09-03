#pragma once

#include <variant>
#include <string>
#include <vector>
#include <vulkan/vulkan.h>

#include "MemoryHolders/Buffer.hpp"
#include "Objects/Texture.hpp"

namespace age {

typedef uint64_t ShaderComponentId;

inline ShaderComponentId __nextId() {
    static ShaderComponentId id = 1;
    id <<= 1; // first bit reserved for color blend (1 enable, 0 disable)
    ShaderComponentId result = id;
    return result;
}

template <typename T>
ShaderComponentId getShaderComponentId() {
	static ShaderComponentId id = __nextId();
	return id;
}

struct ShaderComponentDescription {
    VkShaderStageFlags m_stage;
    VkDescriptorType m_type;
    std::variant<Buffer, Texture> m_descriptor;

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
        m_descriptor = buffer;
        return *this;
    }

    inline ShaderComponentDescription& setTexture(Texture& texture) {
        m_descriptor = texture;
        return *this;
    }
};

struct BlockMember {
    std::string m_member;
    bool m_forward;

    inline BlockMember(std::string member, bool forward = false)
        : m_member(member), m_forward(forward) {} 
};

struct ShaderComponentBuffer {
    std::vector<BlockMember> m_members;
    Buffer m_buffer;

    inline ShaderComponentBuffer& addBlockMember(std::string member, bool forward = false) {
        m_members.push_back( { member, forward } );
        return *this;
    }

    inline ShaderComponentBuffer& setBuffer(Buffer& buffer) {
        m_buffer = buffer;
        return *this;
    }
};

struct ShaderComponentTexture {
    std::string m_name;
    Texture m_texture;

    inline ShaderComponentTexture& setName(const std::string& name) {
        m_name = name;
        return *this;
    }

    inline ShaderComponentTexture& setTexture(Texture& texture) {
        m_texture = texture;
        return *this;
    }
};

struct ShaderComponentForward {
    std::string m_data;
    ShaderComponentForward(const std::string& data)
        : m_data(data) {}
};

struct ShaderComponentInfo {
    std::vector<std::variant<ShaderComponentBuffer, ShaderComponentTexture, ShaderComponentForward>> m_data;
    ShaderComponentDescription m_description;
    ShaderComponentId m_id = 0;
    std::string m_arrayIndex = ""; // if array index is "" - component is not array

    struct {
        std::string rawInsert;
        std::string mainInsert;
    } m_vert;

    struct {
        std::string rawInsert;
        std::string mainInsert;
    } m_frag;

    template<typename T>
    inline ShaderComponentInfo& setId() {
        m_id = getShaderComponentId<T>();
        return *this;
    }

    inline ShaderComponentInfo& setTexture(Texture& t) {
        m_description.m_descriptor = t;
        return *this;
    }

    inline ShaderComponentInfo& setBuffer(Buffer& b) {
        m_description.m_descriptor = b;
        return *this;
    }

    inline ShaderComponentInfo& add(ShaderComponentTexture t) {
        m_description.setStage(VK_SHADER_STAGE_FRAGMENT_BIT);
        m_description.setType(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);
        m_description.setTexture(t.m_texture);
        m_data.push_back(t);
        return *this;
    }

    inline ShaderComponentInfo& add(ShaderComponentBuffer b) {
        m_description.setStage(VK_SHADER_STAGE_VERTEX_BIT);
        m_description.setBuffer(b.m_buffer);
        m_description.setType(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER); // can be changed by Instanced template
        m_data.push_back(b);
        return *this;
    }

    inline ShaderComponentInfo& add(ShaderComponentForward f) {
        m_data.push_back(f);
        return *this;
    }

    inline ShaderComponentInfo& setVertRawInsert(const std::string& s) {
        m_vert.rawInsert = s;
        return *this;
    }

    inline ShaderComponentInfo& setVertMainInsert(const std::string& s) {
        m_vert.mainInsert = s;
        return *this;
    }

    inline ShaderComponentInfo& setFragRawInsert(const std::string& s) {
        m_frag.rawInsert = s;
        return *this;
    }

    inline ShaderComponentInfo& setFragMainInsert(const std::string& s) {
        m_frag.mainInsert = s;
        return *this;
    }
};

} // namespace age
