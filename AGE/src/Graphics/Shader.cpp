#include <vulkan/vulkan.h>

#include "Shader.hpp"
#include "../utils.hpp"
#include "Core/Core.hpp"

namespace age {

void Shader::create(const char* filename, ShaderType type) {
    auto code = readFile(filename);

    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    if (vkCreateShaderModule(core::apiCore.device, &createInfo, nullptr, 
    reinterpret_cast<VkShaderModule*>(&m_shaderModule)) != VK_SUCCESS) {
        std::runtime_error("failed to create shader module");
    }
}

void Shader::destroy() {
    vkDestroyShaderModule(core::apiCore.device, reinterpret_cast<VkShaderModule>(m_shaderModule), nullptr);
}

} // namespace age

