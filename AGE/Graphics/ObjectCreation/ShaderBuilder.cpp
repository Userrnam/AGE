#include <fstream>
#include <utility>

#include "ShaderBuilder.hpp"


namespace age {

inline std::string insertStructName(const std::string& mainInsert, const std::string& structName) {
    std::stringstream ss;
	for (char c : mainInsert) {
		if (c == '?') {
			ss << structName;
		} else {
			ss << c;
		}
	}
	return ss.str();
}

// todo: remove this and use custom variant
static const Inserts& getVariantInserts(const std::variant<ShaderComponentBuffer, ShaderComponentTexture, ShaderComponentForward>& v) {
    if (std::holds_alternative<ShaderComponentBuffer>(v)) {
        return std::get<ShaderComponentBuffer>(v);
    }
    if (std::holds_alternative<ShaderComponentTexture>(v)) {
        return std::get<ShaderComponentTexture>(v);
    }
    if (std::holds_alternative<ShaderComponentForward>(v)) {
        return std::get<ShaderComponentForward>(v);
    }
    
    // just to shut up xcode
    return std::get<ShaderComponentBuffer>(v);;
}

inline std::string getVariableName(const std::string& s) {
    std::stringstream ss(s);
    std::string name;
    ss >> name;
    ss >> name;
    return name;
}

void ShaderBuilder::generateVertexShaderSource(const std::vector<ShaderComponentInfo>& components) {
    m_stream.str(std::string());
    
    m_stream << "#version 450\n#extension GL_ARB_separate_shader_objects : enable\n\n";

    // insert in verticies
    m_stream << "layout(location=0) in vec2 inPosition;\n";
    m_stream << m_vertexAttribs.str() << "\n";

    // insert in global scope
    int binding = 0;
    // block names that will be used in main
    std::vector<std::string> names;
    // fixme:
    names.resize(components.size() * 10);
    std::vector<std::pair<std::string, int>> forwardVariables;
    std::vector<std::string> globalNames;
    for (auto component : components) {
        for (auto &sc : component.m_data) {

            auto& rawInsert = getVariantInserts(sc).m_vertRawInsert;
            if (rawInsert.size() > 0) {
                m_stream << rawInsert << "\n";
            }

            if (std::holds_alternative<ShaderComponentBuffer>(sc)) {
                auto bufferInfo = std::get<ShaderComponentBuffer>(sc);

                std::string name = "n" + std::to_string(binding);
                names[binding] = name + "." + name;
                if (bufferInfo.m_arrayIndex.size()) {
                    names[binding] += bufferInfo.m_arrayIndex;
                }

                // insert struct
                m_stream << "struct YY" << name << " {\n";
                for (auto& member : bufferInfo.m_members) {
                    m_stream << "\t" << member.m_member << ";\n";
                    if (member.m_forward) {
                        forwardVariables.push_back( { member.m_member, binding } );
                    }
                }
                m_stream << "};\n";

                // insert buffer block
                if (bufferInfo.m_arrayIndex.size()) {
                    m_stream << "layout(set=0, binding=" << binding << ") readonly buffer XX" << name << " {\n";
                    m_stream << "\tYY" << name << " " << name << "[];\n";
                } else {
                    m_stream << "layout(set=0, binding=" << binding << ") uniform XX" << name << " {\n";
                    m_stream << "\tYY" << name << " " << name << ";\n";
                }
                m_stream << "} " << name << ";\n\n";

                binding++;
            }

            else if (std::holds_alternative<ShaderComponentTexture>(sc)) {
                binding++;
            }

            else if (std::holds_alternative<ShaderComponentForward>(sc)) {
                auto forward = std::get<ShaderComponentForward>(sc);
                if (forward.m_data.size()) {
                    globalNames.push_back(forward.m_data);
                }
            }
        }
    }

    if (forwardVariables.size() + globalNames.size() > 0) {
        // insert globals struct
        m_stream << "struct Globals {\n";

        // append forwarded globals
        for (auto& v : forwardVariables) {
            m_stream << "\t" << v.first << ";\n";
        }

        // append globals
        for (auto& g : globalNames) {
            m_stream << "\t" << g << ";\n";
        }

        m_stream << "};\n\n";

        // insert globals location layout
        m_stream << "layout(location=0) out Globals globals;\n\n";
    }

    m_stream << "void main() {\n";

    // forward globals
    for (auto& v : forwardVariables) {
        auto name = getVariableName(v.first);
        m_stream << "\tglobals." << name << " = " << names[v.second] << "." << name << ";\n";
    }
    m_stream << "\n";

    m_stream << "\tmat4 transform = mat4(1.0);\n";

    // insert in main
    binding = 0;
    for (auto& component : components) {
        for (auto& elem : component.m_data) {
            auto insert = getVariantInserts(elem).m_vertMainInsert;
            if (insert.size() > 0) {
                m_stream << insertStructName(insert, names[binding]);
            }
            if (!std::holds_alternative<ShaderComponentForward>(elem)) {
                binding++;
            }
        }
    }

    // apply transform
    m_stream << "\tgl_Position = transform * vec4(inPosition, -1.0, 1.0);\n";

    m_stream << "}\n";
}

void ShaderBuilder::generateFragmentShaderSource(const std::vector<ShaderComponentInfo>& components) {
    m_stream.str(std::string());
    
    m_stream << "#version 450\n#extension GL_ARB_separate_shader_objects : enable\n\n";

    m_stream << "layout(location = 0) out vec4 outColor;\n\n";

    // insert in global scope
    int binding = 0;
    // block names that will be used in main
    std::vector<std::string> names;
    names.resize(components.size());
    std::vector<std::pair<std::string, int>> forwardVariables;
    std::vector<std::string> globalNames;
    for (auto component : components) {
        for (auto &sc : component.m_data) {

            auto& rawInsert = getVariantInserts(sc).m_vertRawInsert;
            if (rawInsert.size() > 0) {
                m_stream << rawInsert << "\n";
            }

            if (std::holds_alternative<ShaderComponentBuffer>(sc)) {
                auto bufferInfo = std::get<ShaderComponentBuffer>(sc);
                for (auto& member : bufferInfo.m_members) {
                    if (member.m_forward) {
                        forwardVariables.push_back( { member.m_member, binding } );
                    }
                }
                binding++;
            }

            else if (std::holds_alternative<ShaderComponentTexture>(sc)) {
                auto texInfo = std::get<ShaderComponentTexture>(sc);
                m_stream << "layout(set=0, binding=" << binding << ") uniform sampler2D " << texInfo.m_name << ";\n";
                binding++;
            }

            else if (std::holds_alternative<ShaderComponentForward>(sc)) {
                auto forward = std::get<ShaderComponentForward>(sc);
                if (forward.m_data.size()) {
                    globalNames.push_back(forward.m_data);
                }
            }
        }
    }

    if (forwardVariables.size() + globalNames.size() > 0) {
        // insert globals struct
        m_stream << "struct Globals {\n";

        // append forwarded globals
        for (auto& v : forwardVariables) {
            m_stream << "\t" << v.first << ";\n";
        }

        // append globals
        for (auto& g : globalNames) {
            m_stream << "\t" << g << ";\n";
        }

        m_stream << "};\n\n";

        // insert globals location layout
        m_stream << "layout(location=0) in Globals globals;\n\n";
    }

    m_stream << "void main() {\n";

    m_stream << "\tvec4 fragColor = vec4(1.0);\n";

    // insert in main
    binding = 0;
    for (auto& component : components) {
        for (auto& elem : component.m_data) {
            auto insert = getVariantInserts(elem).m_fragMainInsert;
            m_stream << insertStructName(insert, names[binding]);
            if (!std::holds_alternative<ShaderComponentForward>(elem)) {
                binding++;
            }
        }
    }

    // return color
    m_stream << "\toutColor = fragColor;\n";

    m_stream << "}\n";
}

void ShaderBuilder::saveShader(const std::string& path) {
    std::fstream f;
    f.open(path, std::ios::out);

    auto str = m_stream.str();
    f.write(str.c_str(), str.size());

    f.close();
}

Shader ShaderBuilder::compileVertexShader(const std::vector<ShaderComponentInfo>& components) {
    generateVertexShaderSource(components);
    saveShader("/tmp/tmp.vert");
    auto res = std::system("/usr/local/bin/glslc /tmp/tmp.vert");
    if (res) {
        throw std::runtime_error("[ShaderBuilder]: failed to compile vertex shader");
    }

    Shader out;
    out.create("a.spv");
    out.setEntry("main");
    out.setStage(VK_SHADER_STAGE_VERTEX_BIT);

    std::system("rm /tmp/tmp.vert a.spv");

    return out;
}

Shader ShaderBuilder::compileFragmentShader(const std::vector<ShaderComponentInfo>& components) {
    generateFragmentShaderSource(components);
    saveShader("/tmp/tmp.frag");
    if (std::system("/usr/local/bin/glslc /tmp/tmp.frag")) {
        throw std::runtime_error("[ShaderBuilder]: failed to compile fragment shader");
    }

    Shader out;
    out.create("a.spv");
    out.setEntry("main");
    out.setStage(VK_SHADER_STAGE_FRAGMENT_BIT);

    std::system("rm /tmp/tmp.frag a.spv");

    return out;
}

} // namespace age
