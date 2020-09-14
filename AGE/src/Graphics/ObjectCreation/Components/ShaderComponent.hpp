#pragma once

#include <variant>
#include <string>
#include <vector>
#include <vulkan/vulkan.h>
#include <assert.h>

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

struct Inserts {
    std::string m_vertMainInsert;
    std::string m_vertRawInsert;
    std::string m_fragMainInsert;
    std::string m_fragRawInsert;
};

struct BlockMember {
    std::string m_member;
    bool m_forward;

    inline BlockMember(std::string member, bool forward = false)
        : m_member(member), m_forward(forward) {} 
};

struct ShaderComponentBuffer : public Inserts {
    Buffer m_buffer;
    std::vector<BlockMember> m_members;
    std::string m_arrayIndex = ""; // if array index is "" - component is not array and uses uniform buffer, storage otherwise

    inline ShaderComponentBuffer& addBlockMember(std::string member, bool forward = false) {
        m_members.push_back( { member, forward } );
        return *this;
    }

    inline ShaderComponentBuffer& setBuffer(Buffer& buffer) {
        m_buffer = buffer;
        return *this;
    }

    inline ShaderComponentBuffer& setVertMainInsert(const std::string& s) { m_vertMainInsert = s; return *this; }
    inline ShaderComponentBuffer& setVertRawInsert(const std::string& s) { m_vertRawInsert = s; return *this; }
    inline ShaderComponentBuffer& setFragMainInsert(const std::string& s) { m_fragMainInsert = s; return *this; }
    inline ShaderComponentBuffer& setFragRawInsert(const std::string& s) { m_fragRawInsert = s; return *this; }
};

struct ShaderComponentTexture : public Inserts {
    Texture m_texture;
    std::string m_name;

    inline ShaderComponentTexture& setName(const std::string& name) {
        m_name = name;
        return *this;
    }

    inline ShaderComponentTexture& setTexture(Texture& texture) {
        m_texture = texture;
        return *this;
    }

    inline ShaderComponentTexture& setFragMainInsert(const std::string& s) { m_fragMainInsert = s; return *this; }
    inline ShaderComponentTexture& setFragRawInsert(const std::string& s) { m_fragRawInsert = s; return *this; }
};

struct ShaderComponentForward : public Inserts {
    std::string m_data;

    ShaderComponentForward(const std::string& data)
        : m_data(data) {}
    
    inline ShaderComponentForward& setVertMainInsert(const std::string& s) { m_vertMainInsert = s; return *this; }
    inline ShaderComponentForward& setVertRawInsert(const std::string& s) { m_vertRawInsert = s; return *this; }
    inline ShaderComponentForward& setFragMainInsert(const std::string& s) { m_fragMainInsert = s; return *this; }
    inline ShaderComponentForward& setFragRawInsert(const std::string& s) { m_fragRawInsert = s; return *this; }
};

struct ShaderComponentInfo {
    std::vector<std::variant<ShaderComponentBuffer, ShaderComponentTexture, ShaderComponentForward>> m_data;
    ShaderComponentId m_id = 0;

    template<typename T>
    inline ShaderComponentInfo& setId() {
        m_id = getShaderComponentId<T>();
        return *this;
    }

    inline ShaderComponentInfo& setBuffer(Buffer& b) {
        assert(std::holds_alternative<ShaderComponentBuffer>(m_data[0]));
        std::get<ShaderComponentBuffer>(m_data[0]).m_buffer = b;
        return *this;
    }

    inline ShaderComponentInfo& add(ShaderComponentTexture t) {
        m_data.push_back(t);
        return *this;
    }

    inline ShaderComponentInfo& add(ShaderComponentBuffer b) {
        m_data.push_back(b);
        return *this;
    }

    inline ShaderComponentInfo& add(ShaderComponentForward f) {
        m_data.push_back(f);
        return *this;
    }
};

} // namespace age
