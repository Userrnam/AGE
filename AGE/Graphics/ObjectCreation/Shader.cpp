#include <vulkan/vulkan.h>
#include <stdexcept>

#include "Shader.hpp"
#include "../../Utils/utils.hpp"
#include "../Core/Core.hpp"

namespace age {

Shader& Shader::create(const std::string& filename) {
    auto code = readFile(filename);

    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    if (vkCreateShaderModule(core::apiCore.device, &createInfo, nullptr, &m_shaderModule) != VK_SUCCESS) {
        std::runtime_error("failed to create shader module");
    }

    return *this;
}

void Shader::destroy() {
    vkDestroyShaderModule(core::apiCore.device, m_shaderModule, nullptr);
}

} // namespace age
