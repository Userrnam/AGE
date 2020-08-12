#pragma once

#include <vector>
#include <sstream>

#include "IGraphicsComponent.hpp"

// how to get actual data?
// how to handle functions?
namespace age {

/*
    any bundable component is buffer + some data
    bundle.get<Position>
    what if bundable is
*/

template<int size>
class BundleComponent {
    uint8_t data[size];
    Buffer buffer;

    template <typename T>
    T& getElement(uint32_t offset) {
        return *((T*)(data+offset));
    }
};

// used only for shader generation
class BundleHandler {
    void create(Buffer buf) {
        
    }
};

// creates bundle (multple graphics objects share one buffer)
class BundleComponentHandler : public IGraphicsComponent {
    VkShaderStageFlags m_stageFlags = VK_NULL_HANDLE;
    VkDescriptorType m_type;
    Layout m_structLayout;

    struct {
        Buffer buffer;
        VkDeviceSize bufferSize = VK_NULL_HANDLE;
        VkBufferUsageFlags bufferUsage = VK_NULL_HANDLE;
    } m_buffer;

    struct {
        // location layouts
        std::vector<Layout> layouts;
        std::stringstream main;
    } m_vert;

    struct {
        // location layouts
        std::vector<Layout> layouts;
        std::stringstream main;
    } m_frag;

    // returns true if buffer found
    bool appendLayouts(const std::vector<Layout>& layouts, std::vector<Layout>& locationLayouts) {
        bool bufferFound = false;

        for (auto& layout : layouts) {
            if (layout.m_type == LayoutType::BUFFER) {
                bufferFound = true;
                if (m_structLayout.m_typeName.size() && m_structLayout.m_typeName != layout.m_typeName) {
                    throw std::runtime_error("[BundleComponent]: buffers in bundle must have same type");
                }
                m_structLayout.m_typeName = layout.m_typeName;
                // append members
                for (auto& member : layout.m_members) {
                    m_structLayout.m_members.push_back(member);
                }
                continue;
            }
            if (layout.m_type == LayoutType::LOCATION) {
                locationLayouts.push_back(layout);
                continue;
            }
            throw std::runtime_error("[BundleComponent]: all layouts in bundle component must have type buffer or location");
        }

        m_buffer.buffer.create(
            BufferCreateInfo()
            .setMemoryProperties(VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
            .setUsage(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
        );

        return bufferFound;
    }

public:
    void create(const std::vector<IGraphicsComponent*>& components, const std::string& name="bundle") {
        // first layout is buffer
        m_vert.layouts.resize(1);
        m_structLayout.m_type = LayoutType::BUFFER;

        // if at least one component is used in fragment/vertex shader the whole struct is passed to that shader
        for (auto component : components) {
            // handle vertex
            {
                auto vertexLayouts = component->getVertLayouts();
                if (vertexLayouts.size() > 0) {
                    bool bufferFound = appendLayouts(vertexLayouts, m_vert.layouts);
                    if (bufferFound) {
                        m_stageFlags |= VK_SHADER_STAGE_VERTEX_BIT;
                    }
                }
                m_vert.main << component->getVertMainInsert(name);
            }

            // handle fragment
            {
                auto fragmentLayouts = component->getFragLayouts();
                if (fragmentLayouts.size() > 0) {
                    bool bufferFound = appendLayouts(fragmentLayouts, m_frag.layouts);
                    if (bufferFound) {
                        m_stageFlags |= VK_SHADER_STAGE_FRAGMENT_BIT;
                    }
                }
                m_frag.main << component->getFragMainInsert(name);
            }

            // handle descriptor
            auto gcd = component->getDescription();
            Buffer* bp = std::get<Buffer*>(gcd.m_descriptor);
        }

        // append struct layout
        if (m_stageFlags & VK_SHADER_STAGE_VERTEX_BIT) {
            m_vert.layouts.push_back(m_structLayout);
        }
        if (m_stageFlags & VK_SHADER_STAGE_FRAGMENT_BIT) {
            m_frag.layouts.push_back(m_structLayout);
        }
        if (m_structLayout.m_typeName == "uniform") {
            m_type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        } else {
            m_type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        }
    }

// Interface
    virtual std::vector<Layout> getVertLayouts() override {
        return m_vert.layouts;
    }

    virtual std::string getVertMainInsert(const std::string& structName) override {
        return m_vert.main.str();
    }

    virtual std::vector<Layout> getFragLayouts() override {
        return m_frag.layouts;
    }

    virtual std::string getFragMainInsert(const std::string& structName) override {
        return m_frag.main.str();
    }

    virtual GraphicsComponentDescription getDescription() override {
        GraphicsComponentDescription description;
        // description.descriptor
    }
};

} // namespace age
