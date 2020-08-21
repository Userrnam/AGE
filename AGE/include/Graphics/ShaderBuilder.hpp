#pragma once

#include <string>
#include <sstream>
#include <vector>

#include "Components/ShaderComponent.hpp"
#include "Shader.hpp"

// #include "Containers/Conditional.hpp"

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

    template<typename Head, typename...Tail>
    void generateVertexShaderSource(Head head, Tail... tail) {
        m_components.push_back(head.getInfo());
        generateVertexShaderSource(tail...);
    }
    template<typename Head>
    void generateVertexShaderSource(Head head) {
        m_components.push_back(head.getInfo());
        generateVertexShaderSource(m_components);
        m_components.clear();
    }

    void generateVertexShaderSource(const std::vector<ShaderComponentInfo>& components);
    void generateFragmentShaderSource(const std::vector<ShaderComponentInfo>& components);
    void saveShader(const std::string& path);
    Shader compileVertexShader(const std::vector<ShaderComponentInfo>& components);
    Shader compileFragmentShader(const std::vector<ShaderComponentInfo>& components);
};

} // namespace age
