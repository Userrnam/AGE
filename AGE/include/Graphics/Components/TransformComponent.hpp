#pragma once

#include <sstream>

#include "../Transformable.hpp"
#include "IGraphicsComponent.hpp"

namespace age {

class TransformComponent : public Transformable, public IGraphicsComponent {
    Buffer buffer;
public:
    void create() {
        buffer.create(
            BufferCreateInfo()
            .setMemoryProperties(VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
            .setSize(sizeof(glm::mat4))
            .setUsage(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
        );
    }

    void upload() {
        buffer.load(&m_transform, sizeof(glm::mat4));
    }

    void destroy() {
        buffer.destroy();
    }

// Interface
    virtual std::string getVertStartInsert(int binding, int& outLocation) override {
        std::stringstream ss;
        ss << "layout(set=1,binding=" << binding << ") uniform TransformObject {";
        ss << "mat4 transform;";
        ss << "} transformObject;";

        return ss.str();
    }

    virtual std::string getVertEndInsert() override {
        return "gl_Position *= transformObject.transform;";
    }

    virtual std::string getFragStartInsert(int binding, int& inLocation) override {
        return "";
    }

    virtual std::string getFragEndInsert() override {
        return "";
    }

    virtual GraphicsComponentDescription getDescription() override {
        GraphicsComponentDescription description;
        description.stage = VK_SHADER_STAGE_VERTEX_BIT;
        description.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        description.descriptor = &buffer;

        return description;
    }

};

} // namespace age
    