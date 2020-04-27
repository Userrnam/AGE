#pragma once

#include <string>

namespace age {

namespace vk {

typedef struct ShaderModule_T* ShaderModule;
    
} // namespace vk

enum ShaderType {
    VERTEX_SHADER = 1,
    FRAGMENT_SHADER
};

class Shader {
    vk::ShaderModule m_shaderModule = nullptr;
    ShaderType m_type;
public:
    std::string entryPoint = "main";

    vk::ShaderModule getModule() const { return m_shaderModule; }
    ShaderType getType() const { return m_type; }

    void create(const char* filename, ShaderType type);
    void destroy();
};
    
} // namespace age
