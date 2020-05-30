#pragma once

#include <vector>
#include <vulkan/vulkan.h>

#include "Layer.hpp"
#include "Shader.hpp"
#include "Descriptor.hpp"
#include "Core/RenderPassRef.hpp"
#include "Buffer.hpp"
#include "Vertex.hpp"
#include "Index.hpp"

namespace age {

class DrawableCreateInfo {
    Layer* m_layer;
    bool m_colorBlending = false;
    uint32_t m_instanceCount = 1;

    std::vector<Shader> m_shaders;
    std::vector<Descriptor> m_descriptors;

    struct {
        VkVertexInputBindingDescription bindingDescription;
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
        Buffer buffer;
    } m_vertex;

    struct {
        uint32_t count;
        VkIndexType type;
        Buffer buffer;
    } m_index;

    friend class Drawable;
public:

    inline DrawableCreateInfo& setLayer(Layer* layer) {
        m_layer = layer;
        return *this;
    }

    inline DrawableCreateInfo& setColorBlendEnable(bool flag) {
        m_colorBlending = flag;
        return *this;
    }

    inline DrawableCreateInfo& setIstanceCount(uint32_t count) {
        m_instanceCount = count;
        return *this;
    }

    inline DrawableCreateInfo& addShader(Shader& shader) {
        m_shaders.push_back(shader);
        return *this;
    }

    inline DrawableCreateInfo& addDescriptor(const Descriptor& descriptor) {
        m_descriptors.push_back(descriptor);
        return *this;
    }

    inline DrawableCreateInfo& setVertexBuffer(const Buffer& buffer) {
        m_vertex.buffer = buffer;
        return *this;
    }

    template<class T>
    inline DrawableCreateInfo& setVertexProperties() {
        Vertex<T>::fillBinding(m_vertex.bindingDescription);
        Vertex<T>::fillAttributes(m_vertex.attributeDescriptions);
        return *this;
    }

    template<class T>
    inline DrawableCreateInfo& loadVerticies(const std::vector<Vertex<T>>& verticies) {
        m_vertex.buffer.create(
            VertexBufferCreateInfo().setSize(verticies.size() * sizeof(T))
        );
        m_vertex.buffer.loadDeviceLocal(verticies.data(), verticies.size() * sizeof(T));
        Vertex<T>::fillBinding(m_vertex.bindingDescription);
        Vertex<T>::fillAttributes(m_vertex.attributeDescriptions);

        return *this;
    }

    inline DrawableCreateInfo& setIndexBuffer(const Buffer& buffer) {
        m_index.buffer = buffer;
        return *this;
    }

    inline DrawableCreateInfo& setIndexType(VkIndexType type) {
        m_index.type = type;
        return *this;
    }

    inline DrawableCreateInfo& setIndexCount(uint32_t count) {
        m_index.count = count;
        return *this;
    }

    inline DrawableCreateInfo& loadIndicies(const std::vector<Index16>& indicies) {
        m_index.buffer.create(
            IndexBufferCreateInfo().setSize(indicies.size() * sizeof(Index16))
        );
        m_index.buffer.loadDeviceLocal(indicies.data(), indicies.size() * sizeof(Index16));

        m_index.type = VK_INDEX_TYPE_UINT16;
        m_index.count = indicies.size();

        return *this;
    }

    inline DrawableCreateInfo& loadIndicies(const std::vector<Index32>& indicies) {
        m_index.buffer.create(
            IndexBufferCreateInfo().setSize(indicies.size() * sizeof(Index32))
        );
        m_index.buffer.loadDeviceLocal(indicies.data(), indicies.size() * sizeof(Index32));

        m_index.type = VK_INDEX_TYPE_UINT32;
        m_index.count = indicies.size();

        return *this;
    }

};

class Drawable {
protected:
    core::RenderPassRef* m_renderPass;
    VkPipelineLayout m_pipelineLayout;
    VkPipeline m_pipeline;
    std::vector<uint32_t> m_poolIndicies;
    std::vector<VkDescriptorSet> m_descriptorSets;

    struct {
        Buffer buffer;
    } m_vertex;

    struct {
        uint32_t count;
        VkIndexType type;
        Buffer buffer;
    } m_index;

    uint32_t m_instanceCount;

    void createDrawable(const DrawableCreateInfo& createInfo);
public:
    void draw(int i);
};

void destroyPipeline(VkPipeline pipeline);

} // namespace age
