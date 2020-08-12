#pragma once

#include <vector>
#include <stdint.h>

#include "Containers/DynamicBuffer.hpp"
#include "IGraphicsComponent.hpp"

namespace age {

/*
    if component is instanced it looks like this:

    struct YYname {
        field1;
        field2;
        ...
    };

    layout (set=1, binding = .) readonly buffer XXname {
        YYname instances[];
    } nameBlock;

    // in main
    YYname name = nameBlock.instances[gl_InstanceIndex];

    // then go on like usual. nothing else changes
    ... 

    if everything is struct the differences are layout type is readonly buffer,
    name is nameBlock.instances[gl_InstanceIndex];

*/

/*

template <typename T>
class Any {
    Buffer buffer;
    T data;
public:
    upload() {...}
    setData()
    getData()

};

*/

struct InstancedComponent {
    DynamicBuffer m_instances;
    Buffer m_buffer;

    void upload() {
        m_buffer.load(m_instances.data(), m_instances.size());
    }
};

class InstancedStructInfo {
    std::vector<std::string> m_fields;
public:
    inline const std::vector<std::string>& getFields() const {
        return m_fields;
    }

    inline InstancedStructInfo& addField(const std::string& field) {
        m_fields.push_back(field);
        return *this;
    }
};

// used only to generate shader
class InstancedComponentDescription : public IGraphicsComponent {
    InstancedComponent m_component;
    VkShaderStageFlags m_stage;
    std::string m_rawInsert; // struct defenition
    std::string m_vertMainInsert;
    std::string m_fragMainInsert;
    std::string m_name;
public:
    void create(const std::string& name, const InstancedStructInfo& info, const std::string& vertMainInsert, const std::string& fragMainInsert, uint32_t bufferSize = 1024, VkShaderStageFlags stage = VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_VERTEX_BIT) {
        m_stage = stage;
        m_name = name;
        m_vertMainInsert = vertMainInsert;
        m_fragMainInsert = fragMainInsert;

        m_component.m_buffer.create(
            BufferCreateInfo()
            .setMemoryProperties(VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
            .setUsage(VK_BUFFER_USAGE_STORAGE_BUFFER_BIT)
            .setSize(bufferSize)
        );

        std::stringstream ss;
        ss << "struct YY" << name << " {\n";

        auto& fields = info.getFields();
        for (auto& field : fields) {
            ss << "\t" << field << ";\n";
        }

        ss << "};\n";

        m_rawInsert = ss.str();
    }

    InstancedComponent getComponent() {
        return m_component;
    }

    virtual std::string getVertRawInsert() override {
        if (m_vertMainInsert.size() > 0) {
            return m_rawInsert;
        }
        return "";
    }

    virtual std::vector<Layout> getVertLayouts() override {
        std::vector<Layout> layouts;
        std::stringstream ss;
        ss << "YY" << m_name << "instances[]";
        layouts.push_back(
            Layout()
            .addBlockMember(
                ss.str()
            )
        );
        return layouts;
    }

    virtual std::string getVertMainInsert(const std::string& structName) override {
        return m_vertMainInsert;
    }

    virtual std::string getFragRawInsert() override {
        if (m_fragMainInsert.size() > 0) {
            return m_rawInsert;
        }
        return "";
    }

    virtual std::vector<Layout> getFragLayouts() override {
        std::vector<Layout> layouts;
        return layouts;
    }

    virtual std::string getFragMainInsert(const std::string& structName) override {
        std::stringstream ss;
        return ss.str();
    }

    virtual GraphicsComponentDescription getDescription() override {
        GraphicsComponentDescription description;
        description.setBuffer(m_component.m_buffer);
        description.setStage(m_stage);
        description.setType(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);
        return description;
    }
};

// struct InstancedComponent {
//     std::vector<T> m_instances;
//     Buffer buffer;
//     uint32_t maxSize = 0;

//     void upload() {
//         buffer.load(m_instances.data(), m_instances.size());
//     }

//     inline void addInstance(T instance) {
//         m_instances.push_back(instance);
//     }
// };

} // namespace age
