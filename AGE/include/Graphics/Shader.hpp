#pragma once

#include <vulkan/vulkan.h>
#include <string>

namespace age {

class Shader {
    VkShaderModule m_shaderModule;
public:
    std::string entry;
    VkShaderStageFlagBits stage;

    VkShaderModule getShaderModule() const { return m_shaderModule; }

    void create(const std::string& filename);
    void destroy();
};

} // namespace age

