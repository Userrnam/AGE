#include <fstream>

#include "ShaderBuilder.hpp"


namespace age {

void ShaderBuilder::generateVertexShaderSource(const std::vector<IGraphicsComponent*>& components) {
    m_stream << "#version 450\n#extension GL_ARB_separate_shader_objects : enable\n\n";

    // insert in verticies
    m_stream << "layout(location=0) in vec2 inPosition;\n\n";

    // insert camera        
    m_stream << "layout(set = 0, binding = 0) uniform CameraObject {\n";
    m_stream << "\tmat4 transform;\n";
    m_stream << "\tvec4 time;\n";
    m_stream << "} camera;\n\n";

    // insert in global scope
    int i = 0;
    int outLocation = 0;
    for (auto component : components) {
        auto insert = component->getVertStartInsert(i, outLocation);
        m_stream << insert;
        if (insert.size() > 0) {
            m_stream << "\n";
        }
        i++;
    }

    m_stream << "void main() {\n";

    // apply camera transform
    m_stream << "\tmat4 transform = camera.transform;\n";

    // insert in main
    for (auto component : components) {
        auto insert = component->getVertEndInsert();
        if (insert.size() > 0) {
            m_stream << "\t" << insert;
        }
    }

    // apply transform
    m_stream << "\tgl_Position = transform * vec4(inPosition, -1.0, 1.0);\n";

    m_stream << "}\n";
}

void ShaderBuilder::generateFragmentShaderSource(const std::vector<IGraphicsComponent*>& components) {
    m_stream << "#version 450\n#extension GL_ARB_separate_shader_objects : enable\n\n";
    m_stream << "layout(location=0) out vec4 fragColor;\n\n";

    // insert in global scope
    int i = 0;
    int inLocation = 0;
    for (auto component : components) {
        auto insert = component->getFragStartInsert(i, inLocation);
        m_stream << insert;
        if (insert.size() > 0) {
            m_stream << "\n";
        }
        i++;
    }

    m_stream << "void main() {\n";
    m_stream << "\tfragColor = vec4(1.0,1.0,1.0,1.0);\n";

    // insert in main
    for (auto component : components) {
        auto insert = component->getFragEndInsert();
        if (insert.size() > 0) {
            m_stream << "\t" << component->getFragEndInsert();
        }
    }

    m_stream << "}\n";
}

void ShaderBuilder::saveShader(const std::string& path) {
    std::fstream f;
    f.open(path, std::ios::out);

    auto str = m_stream.str();
    f.write(str.c_str(), str.size());

    f.close();
}

} // namespace age
