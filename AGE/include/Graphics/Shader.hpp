#pragma once

#include <vulkan/vulkan.h>
#include <string>
#include <vector>

#include "Debug.hpp"

namespace age {

class ShaderSpecialization {
    uint32_t m_index = 0;
    std::vector<VkSpecializationMapEntry> m_entries;
    std::vector<uint8_t> m_data;

    friend class Drawable;
public:

    template<typename T>
    ShaderSpecialization& add(T val) {
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

        return *this;
    }
};

class Shader {
    VkShaderModule m_shaderModule;
    std::string m_entry = "main";
    VkShaderStageFlagBits m_stage;
    ShaderSpecialization m_specialization;

#ifndef NDEBUG
    bool stageSet = false;
#endif

public:
    Shader& create(const std::string& filename);
    void destroy();

    VkShaderModule getModule() const { return m_shaderModule; }
    const std::string& getEntry() const { return m_entry; }
    const ShaderSpecialization& getSpecialization() const { return m_specialization; }
    VkShaderStageFlagBits getStage() const {
        ASSERT(stageSet, "Shader stage has not been set")
        return m_stage;
    }

    inline Shader& setStage(VkShaderStageFlagBits stage) {
#ifndef NDEBUG
        stageSet = true;
#endif
        m_stage = stage;
        return *this;
    }

    inline Shader& setEntry(const std::string& entry) {
        m_entry = entry;
        return *this;
    }

    inline Shader& setSpecialization(const ShaderSpecialization& specialization) {
        m_specialization = specialization;
        return *this;
    }
};

} // namespace age
