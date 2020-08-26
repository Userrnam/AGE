#pragma once

#include <string>
#include <sstream>
#include <vector>

#include "Components/ShaderComponent.hpp"
#include "ComponentCollector.hpp"
#include "Shader.hpp"

namespace age {

// Generates shader source code
class ShaderBuilder {
    std::stringstream m_stream;
    std::stringstream m_vertexAttribs;
    std::vector<ShaderComponentInfo> m_components;
public:
    void addVertexAttribute(const std::string& attr) {
        m_vertexAttribs << attr << "\n";
    }

    template<typename... Args>
    void generateVertexShaderSource(Args... components) {
        collectComponents(m_components, components...);
        generateVertexShaderSource(m_components);
        m_components.clear();
    }

    template<typename... Args>
    Shader compileVertexShader(Args... components) {
        collectComponents(m_components, components...);
        auto shader = compileVertexShader(m_components);
        m_components.clear();
        return shader;
    }

    template<typename... Args>
    void generateFragmentShaderSource(Args... components) {
        collectComponents(m_components, components...);
        generateFragmentShaderSource(m_components);
        m_components.clear();
    }

    template<typename... Args>
    Shader compileFragmentShader(Args... components) {
        collectComponents(m_components, components...);
        auto shader = compileFragmentShader(m_components);
        m_components.clear();
        return shader;
    }

    void generateVertexShaderSource(const std::vector<ShaderComponentInfo>& components);
    void generateFragmentShaderSource(const std::vector<ShaderComponentInfo>& components);
    void saveShader(const std::string& path);
    Shader compileVertexShader(const std::vector<ShaderComponentInfo>& components);
    Shader compileFragmentShader(const std::vector<ShaderComponentInfo>& components);
};

} // namespace age
