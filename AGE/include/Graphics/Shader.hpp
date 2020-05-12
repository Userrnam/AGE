#pragma once

#include <vulkan/vulkan.h>
#include <string>
#include <vector>

namespace age {

class ShaderSpecialization {
    uint32_t m_index = 0;
public:
    std::vector<VkSpecializationMapEntry> entries;
    std::vector<uint8_t> data;

    template<typename T>
    void add(T val) {
        VkSpecializationMapEntry entry;
        entry.constantID = m_index;
        entry.offset = data.size() * sizeof(uint8_t);
        entry.size = sizeof(T);
        entries.push_back(entry);

        // resize
        size_t prevSize = data.size();
        data.resize(prevSize + sizeof(T));
        memcpy(data.data() + prevSize, &val, sizeof(T));

        m_index++;
    }
};

// if data.size() != 0 use specialization

class Shader {
    VkShaderModule m_shaderModule;
public:
    ShaderSpecialization specialization;
    std::string entry;
    VkShaderStageFlagBits stage;

    VkShaderModule getShaderModule() const { return m_shaderModule; }

    void create(const std::string& filename);
    void destroy();
};

} // namespace age

