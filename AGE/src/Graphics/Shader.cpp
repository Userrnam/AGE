#include <vulkan/vulkan.h>

#include "Shader.hpp"
#include "Utils/utils.hpp"
#include "Core/Core.hpp"

namespace age {

namespace __ {

void Shader::create(const std::string& filename) {
    auto code = readFile(filename);

    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    if (vkCreateShaderModule(core::apiCore.device, &createInfo, nullptr, &m_shaderModule) != VK_SUCCESS) {
        std::runtime_error("failed to create shader module");
    }
}

void Shader::destroy() {
    vkDestroyShaderModule(core::apiCore.device, m_shaderModule, nullptr);
}

}

Shaders& Shaders::addVertexShader(const std::string& path, const std::string& entry, const ShaderSpecialization& specialization) {
    __::Shader shader;
    shader.m_entry = entry;
    shader.m_specialization = specialization;
    shader.m_stage = VK_SHADER_STAGE_VERTEX_BIT;
    shader.create(path);

    m_shaders.push_back(shader);

    return *this;
}

Shaders& Shaders::addFragmentShader(const std::string& path, const std::string& entry, const ShaderSpecialization& specialization) {
    __::Shader shader;
    shader.m_entry = entry;
    shader.m_specialization = specialization;
    shader.m_stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    shader.create(path);

    m_shaders.push_back(shader);

    return *this;
}

void Shaders::destroy() {
    for (auto& shader: m_shaders) {
        shader.destroy();
    }
}

} // namespace age
