#pragma once

#include <string>
#include <sstream>
#include <vector>

#include "Components/IGraphicsComponent.hpp"
#include "Shader.hpp"

#include "Containers/Conditional.hpp"

namespace age {

enum ShaderComponentOptions {
    NO_INSTANCED = 0,
    INSTANCED = 1
};

// Generates shader source code
class ShaderBuilder {
    std::stringstream m_stream;
    std::stringstream m_vertexAttribs;
    std::vector<Conditional<IGraphicsComponent*>> m_components;
public:
    void addVertexAttribute(const std::string& attr) {
        m_vertexAttribs << attr << "\n";
    }

    template<typename Head, typename...Tail>
    void generateVertexShaderSource(Head head, Tail... tail) {
        m_components.push_back(&head);
        generateVertexShaderSource(tail...);
    }

    template<typename Head>
    void generateVertexShaderSource(Head head) {
        m_components.push_back(&head);
        generateVertexShaderSource(m_components);
        m_components.clear();
    }

    void generateVertexShaderSource(const std::vector<Conditional<IGraphicsComponent*>>& components);
    void generateFragmentShaderSource(const std::vector<Conditional<IGraphicsComponent*>>& components);
    void saveShader(const std::string& path);

    Shader compileVertexShader(const std::vector<Conditional<IGraphicsComponent*>>& components);
    Shader compileFragmentShader(const std::vector<Conditional<IGraphicsComponent*>>& components);
};

} // namespace age
