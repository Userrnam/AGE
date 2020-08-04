#pragma once

#include <string>
#include <sstream>
#include <vector>

#include "Components/IGraphicsComponent.hpp"


namespace age {

// Generates shader source code
class ShaderBuilder {
    std::stringstream m_stream;
public:
    void generateVertexShaderSource(const std::vector<IGraphicsComponent*>& components);
    void generateFragmentShaderSource(const std::vector<IGraphicsComponent*>& components);
    void saveShader(const std::string& path);

    void clear() {
        m_stream.str(std::string());
    }
};

} // namespace age
