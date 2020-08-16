#include <fstream>

#include "ShaderBuilder.hpp"


namespace age {

/*

maybe automatically generate struct and pass it to fragment shader via layout?
so all buffers are only accessed in vertex shader
use some global name for this layout (for example globals)

struct {

} unknown;

? is replaced by shaderBuilder
// in main
fragment.color = ?.color;

*/

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

void ShaderBuilder::generateVertexShaderSource(const std::vector<Conditional<IGraphicsComponent*>>& components) {
    m_stream.str(std::string());
    
    m_stream << "#version 450\n#extension GL_ARB_separate_shader_objects : enable\n\n";

    // insert in verticies
    m_stream << "layout(location=0) in vec2 inPosition;\n";
    m_stream << m_vertexAttribs.str() << "\n";

    // insert globals
    m_stream << "layout(set = 0, binding = 0) uniform CameraObject {\n";
    m_stream << "\tmat4 cameraTransform;\n";
    // maybe pass it to fragment shader via location layout?
    m_stream << "\tvec2 resolution;\n";
    m_stream << "\tfloat time;\n";
    m_stream << "\tfloat deltaTime;\n";
    m_stream << "} globals;\n\n";

    // insert in global scope
    int binding = 0;
    int outLocation = 0;
    // names that will be used in main
    std::vector<std::string> names;
    names.resize(components.size());
    for (auto component : components) {
        auto rawInsert = component.data->getVertRawInsert();
        if (rawInsert.size() > 0) {
            m_stream << component.data->getVertRawInsert() << "\n";
        }

        auto layouts = component.data->getVertLayouts();
        bool bindingFound = false;
        for (auto& layout : layouts) {
            switch (layout.m_type) {
            case LayoutType::BUFFER:
                if (bindingFound) {
                    throw std::runtime_error("[ShaderBuilder]: Graphics component can contain only 1 binding");
                }

                if (component.condition == INSTANCED) {
                    names[binding] = layout.m_name + "." + layout.m_name + "[instanceIndex]";
                } else {
                    names[binding] = layout.m_name + "." + layout.m_name;
                }
                m_stream << "struct YY" << layout.m_name << " {\n";
                for (auto& member : layout.m_members) {
                    m_stream << "\t" << member << ";\n";
                }
                m_stream << "};\n";

                if (component.condition == INSTANCED) {
                    m_stream << "layout(set=1, binding=" << binding << ") readonly buffer XX" << layout.m_name << " {\n";
                    m_stream << "\tYY" << layout.m_name << " " << layout.m_name << "[];\n";
                } else {
                    m_stream << "layout(set=1, binding=" << binding << ") " << layout.m_typeName << " XX" << layout.m_name << " {\n";
                    m_stream << "\tYY" << layout.m_name << " " << layout.m_name << ";\n";
                }

                m_stream << "} " << layout.m_name << ";\n";

                bindingFound = true;
                break;
            case LayoutType::LOCATION:
                // change: append typename and name to struct that will be generated
                m_stream << "layout(location=" << outLocation << ") out " << layout.m_typeName << " " << layout.m_name << ";\n";
                outLocation++;
                break;
            case LayoutType::SAMPLER:
                throw std::runtime_error("[ShaderBuilder]: Sampler can only be in fragment shader");
                break;
            case LayoutType::UNDEFINED:
                throw std::runtime_error("[ShaderBuilder]: Type was not set for GraphicsComponent");
                break;
            }
        }
        if (layouts.size()) {
            m_stream << "\n";
        }
        binding++;
    }

    m_stream << "void main() {\n";

    // apply camera transform
    m_stream << "\tmat4 transform = globals.cameraTransform;\n";

    // insert in main
    binding = 0;
    for (auto component : components) {
        // auto insert = component.data->getVertMainInsert(names[binding]);
        auto insert = component.data->getVertMainInsert();
        if (insert.size() > 0) {
            m_stream << "\t" << insertStructName(insert, names[binding]);
        }
        binding++;
    }

    // apply transform
    m_stream << "\tgl_Position = transform * vec4(inPosition, -1.0, 1.0);\n";

    m_stream << "}\n";
}

void ShaderBuilder::generateFragmentShaderSource(const std::vector<Conditional<IGraphicsComponent*>>& components) {
    m_stream.str(std::string());

    m_stream << "#version 450\n#extension GL_ARB_separate_shader_objects : enable\n\n";
    m_stream << "layout(location=0) out vec4 fragColor;\n\n";
    m_stream << "layout(location=0) in flat uint instanceIndex;\n\n";

    // insert globals
    m_stream << "layout(set = 0, binding = 0) uniform CameraObject {\n";
    m_stream << "\tmat4 cameraTransform;\n";
    m_stream << "\tvec2 resolution;\n";
    m_stream << "\tfloat time;\n";
    m_stream << "\tfloat deltaTime;\n";
    m_stream << "} globals;\n\n";

    // insert in global scope
    int binding = 0;
    int inLocation = 0;
    std::vector<std::string> names;
    names.resize(components.size());
    for (auto component : components) {
        auto rawInsert = component.data->getFragRawInsert();
        if (rawInsert.size() > 0) {
            m_stream << component.data->getFragRawInsert() << "\n";
        }
        auto layouts = component.data->getFragLayouts();
        bool bindingFound = false;
        for (auto& layout : layouts) {
            switch (layout.m_type) {
            case LayoutType::BUFFER:
                if (bindingFound) {
                    throw std::runtime_error("[ShaderBuilder]: Graphics component can contain only 1 binding");
                }

                if (component.condition == INSTANCED) {
                    names[binding] = layout.m_name + "." + layout.m_name + "[instanceIndex]";
                } else {
                    names[binding] = layout.m_name + "." + layout.m_name;
                }
                m_stream << "struct YY" << layout.m_name << " {\n";
                for (auto& member : layout.m_members) {
                    m_stream << "\t" << member << ";\n";
                }
                m_stream << "};\n";

                if (component.condition == INSTANCED) {
                    m_stream << "layout(set=1, binding=" << binding << ") readonly buffer XX" << layout.m_name << " {\n";
                    m_stream << "\tYY" << layout.m_name << " " << layout.m_name << "[];\n";
                } else {
                    m_stream << "layout(set=1, binding=" << binding << ") " << layout.m_typeName << " XX" << layout.m_name << " {\n";
                    m_stream << "\tYY" << layout.m_name << " " << layout.m_name << ";\n";
                }

                m_stream << "} " << layout.m_name << ";\n";

                bindingFound = true;
                break;
            case LayoutType::LOCATION:
                m_stream << "layout(location=" << inLocation << ") in " << layout.m_typeName << " " << layout.m_name << ";\n";
                inLocation++;
                break;
            case LayoutType::SAMPLER:
                if (bindingFound) {
                    throw std::runtime_error("[ShaderBuilder]: Graphics component can contain only 1 binding");
                }
                m_stream << "layout(set=1, binding=" << binding << ") " << layout.m_typeName << " " << layout.m_name << ";\n";
                bindingFound = true;
                break;
            case LayoutType::UNDEFINED:
                throw std::runtime_error("[ShaderBuilder]: Type was not set for GraphicsComponent");
                break;
            }
        }
        if (layouts.size()) {
            m_stream << "\n";
        }
        binding++;
    }

    m_stream << "void main() {\n";
    m_stream << "\tfragColor = vec4(1.0,1.0,1.0,1.0);\n";

    // insert in main
    binding = 0;
    for (auto component : components) {
        // auto insert = component.data->getFragMainInsert(names[binding]);
        auto insert = component.data->getFragMainInsert();
        if (insert.size() > 0) {
            m_stream << "\t" << insertStructName(insert, names[binding]);
        }
        binding++;
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

Shader ShaderBuilder::compileVertexShader(const std::vector<Conditional<IGraphicsComponent*>>& components) {
    generateVertexShaderSource(components);
    saveShader("tmp.vert");
    if (std::system("glslc tmp.vert")) {
        std::system("rm tmp.vert a.spv");
        throw std::runtime_error("[ShaderBuilder]: failed to compile vertex shader");
    }

    Shader out;
    out.create("a.spv");
    out.setEntry("main");
    out.setStage(VK_SHADER_STAGE_VERTEX_BIT);

    std::system("rm tmp.vert a.spv");

    return out;
}

Shader ShaderBuilder::compileFragmentShader(const std::vector<Conditional<IGraphicsComponent*>>& components) {
    generateFragmentShaderSource(components);
    saveShader("tmp.frag");
    if (std::system("glslc tmp.frag")) {
        std::system("rm tmp.frag a.spv");
        throw std::runtime_error("[ShaderBuilder]: failed to compile fragment shader");
    }

    Shader out;
    out.create("a.spv");
    out.setEntry("main");
    out.setStage(VK_SHADER_STAGE_FRAGMENT_BIT);

    std::system("rm tmp.frag a.spv");

    return out;
}

} // namespace age
