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
public:
    void addVertexAttribute(const std::string& attr) {
        m_vertexAttribs << attr << "\n";
    }
    void generateVertexShaderSource(const std::vector<Conditional<IGraphicsComponent*>>& components);
    void generateFragmentShaderSource(const std::vector<Conditional<IGraphicsComponent*>>& components);
    void saveShader(const std::string& path);

    Shader compileVertexShader(const std::vector<Conditional<IGraphicsComponent*>>& components);
    Shader compileFragmentShader(const std::vector<Conditional<IGraphicsComponent*>>& components);
};

} // namespace age
