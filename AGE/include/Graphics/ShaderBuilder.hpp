#pragma once

#include <string>
#include <sstream>
#include <vector>

#include "Components/ShaderComponent.hpp"
#include "Shader.hpp"

namespace age {

// Generates shader source code
class ShaderBuilder {
    std::stringstream m_stream;
    std::stringstream m_vertexAttribs;
    std::vector<ShaderComponentInfo> m_components;

    template<typename Head, typename...Tail>
    void collectComponents(Head head, Tail... tail) {
        m_components.push_back(head.getInfo());
        collectComponents(tail...);
    }
    template<typename Head>
    void collectComponents(Head head) {
        m_components.push_back(head.getInfo());
    }
public:
    void addVertexAttribute(const std::string& attr) {
        m_vertexAttribs << attr << "\n";
    }

    template<typename Head, typename...Tail>
    void generateVertexShaderSource(Head head, Tail... tail) {
        collectComponents(head, tail...);
        generateVertexShaderSource(m_components);
        m_components.clear();
    }

    template<typename Head, typename...Tail>
    Shader compileVertexShader(Head head, Tail... tail) {
        collectComponents(head, tail...);
        auto shader = compileVertexShader(m_components);
        m_components.clear();
        return shader;
    }

    template<typename Head, typename...Tail>
    void generateFragmentShaderSource(Head head, Tail... tail) {
        collectComponents(head, tail...);
        generateFragmentShaderSource(m_components);
        m_components.clear();
    }

    template<typename Head, typename...Tail>
    Shader compileFragmentShader(Head head, Tail... tail) {
        collectComponents(head, tail...);
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
