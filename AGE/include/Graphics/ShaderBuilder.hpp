#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include "Components/IGraphicsComponent.hpp"


namespace age {

// Generates shader source code
class ShaderBuilder {
    std::stringstream stream;
public:
    void generateVertexShaderSource(const std::vector<IGraphicsComponent*>& components) {
        stream << "#version 450\n#extension GL_ARB_separate_shader_objects : enable\n";

        // insert in verticies
        stream << "layout(location=0) in vec4 inPosition;";

        // insert camera        
        stream << "layout(set = 0, binding = 0) uniform CameraObject {";
        stream << "mat4 transform;";
        stream << "vec4 time;";
        stream << "} camera;";

        // insert in global scope
        int i = 0;
        int outLocation = 0;
        for (auto component : components) {
            stream << component->getVertStartInsert(i, outLocation);
            i++;
        }

        stream << "void main() {";

        // apply camera transform
        stream << "gl_Position = camera.transform;";

        // insert in main
        for (auto component : components) {
            stream << component->getVertEndInsert();
        }

        // apply transform
        stream << "gl_Position *= vec4(inPosition, -1.0, 1.0);";

        stream << "}\n";
    }

    void generateFragmentShaderSource(const std::vector<IGraphicsComponent*>& components) {
        stream << "#version 450\n#extension GL_ARB_separate_shader_objects : enable\n";
        stream << "layout(location=0) out vec4 fragColor;";

        // insert in global scope
        int i = 0;
        int inLocation = 0;
        for (auto component : components) {
            stream << component->getFragStartInsert(i, inLocation);
            i++;
        }

        stream << "void main() {";
        stream << "fragColor = vec4(1.0,1.0,1.0,1.0);";

        // insert in main
        for (auto component : components) {
            stream << component->getFragEndInsert();
        }

        stream << "}\n";
    }

    void clear() {
        stream.clear();
    }

    void saveShader(const std::string& path) {
        std::fstream f;
        f.open(path, std::ios::out);

        auto str = stream.str();
        f.write(str.c_str(), str.size());

        f.close();
    }
};

} // namespace age
