#pragma once

#include <vulkan/vulkan.h>
#include <string>
#include <vector>

namespace age {

class ShaderSpecialization {
    uint32_t m_index = 0;
    std::vector<VkSpecializationMapEntry> m_entries;
    std::vector<uint8_t> m_data;

    friend class Object;
public:

    template<typename T>
    void add(T val) {
        VkSpecializationMapEntry entry;
        entry.constantID = m_index;
        entry.offset = m_data.size() * sizeof(uint8_t);
        entry.size = sizeof(T);
        m_entries.push_back(entry);

        // resize
        size_t prevSize = m_data.size();
        m_data.resize(prevSize + sizeof(T));
        memcpy(m_data.data() + prevSize, &val, sizeof(T));

        m_index++;
    }
};

// if specialization.data.size() != 0 use specialization

class Shaders;

namespace __ {

class Shader {
    VkShaderModule m_shaderModule;
    std::string m_entry;
    VkShaderStageFlagBits m_stage;
    ShaderSpecialization m_specialization;

    friend class age::Shaders;
public:

    VkShaderModule getModule() const { return m_shaderModule; }
    const std::string& getEntry() const { return m_entry; }
    VkShaderStageFlagBits getStage() const { return m_stage; }
    const ShaderSpecialization& getSpecialization() const { return m_specialization; }

    void create(const std::string& filename);
    void destroy();
};

}

class Shaders {
    std::vector<__::Shader> m_shaders;
public:
    size_t size() const { return m_shaders.size(); }
    __::Shader& operator[](size_t i) { return m_shaders[i]; }
    const __::Shader& operator[](size_t i) const { return m_shaders[i]; }

    Shaders& addVertexShader(const std::string& path, const std::string& entry = "main", const ShaderSpecialization& specialization = {});
    Shaders& addFragmentShader(const std::string& path, const std::string& entry = "main", const ShaderSpecialization& specialization = {});

    void destroy();
};

} // namespace age
