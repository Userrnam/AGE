#pragma once

#include "Containers/Shared.hpp"
#include "../Memory/MemoryId.hpp"

#include "Vertex.hpp"
#include "Index.hpp"

namespace age {

typedef uint32_t ShapeId;

enum BasicShape {
    RECTANGLE_SHAPE = 0
};

struct ShapePipelineCreateDescription {
    VkVertexInputBindingDescription bindingDescription;
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
};

class ShapeCreateInfo {
    struct {
        ShapePipelineCreateDescription description;
        std::vector<uint8_t> data;
    } m_vertex;

    struct {
        uint32_t count;
        VkIndexType type;
        std::vector<uint8_t> data;
    } m_index;

    friend class Shape;
public:
    template<class T>
    inline ShapeCreateInfo& loadVerticies(const std::vector<Vertex<T>>& verticies) {
        m_vertex.data.resize(verticies.size() * sizeof(T));
        memcpy(m_vertex.data.data(), verticies.data(), m_vertex.data.size());
        Vertex<T>::fillBinding(m_vertex.description.bindingDescription);
        Vertex<T>::fillAttributes(m_vertex.description.attributeDescriptions);

        return *this;
    }

    inline ShapeCreateInfo& loadIndicies(const std::vector<Index16>& indicies) {
        m_index.data.resize(indicies.size() * sizeof(Index16));
        memcpy(m_index.data.data(), indicies.data(), m_index.data.size());

        m_index.type = VK_INDEX_TYPE_UINT16;
        m_index.count = indicies.size();

        return *this;
    }

    inline ShapeCreateInfo& loadIndicies(const std::vector<Index32>& indicies) {
        m_index.data.resize(indicies.size() * sizeof(Index16));
        memcpy(m_index.data.data(), indicies.data(), m_index.data.size());

        m_index.type = VK_INDEX_TYPE_UINT32;
        m_index.count = indicies.size();

        return *this;
    }
};

class ShapeRenderInfo {
    core::MemoryId m_vertexMemoryId;
    core::MemoryId m_indexMemoryId;
    VkIndexType m_indexType;
    uint32_t m_indexCount;

    inline VkBuffer getVertexBuffer() const { return m_vertexMemoryId.buffer; }
    inline VkDeviceSize getVertexOffset() const { return m_vertexMemoryId.address; }

    inline VkBuffer getIndexBuffer() const { return m_indexMemoryId.buffer; }
    inline VkDeviceSize getIndexOffset() const { return m_indexMemoryId.address; }
    inline uint32_t getIndexCount() const { return m_indexCount; }

    friend class Shape;
    friend class Drawable;
};

class Shape {
    static void createManager();
    static void destroyManager();
    friend class Application;
public:
    static ShapeId create(const ShapeCreateInfo& info);
    static ShapeRenderInfo get(ShapeId id);
    static const ShapePipelineCreateDescription& getPipelineCreateDescription(ShapeId id);
};

} // namespace age
