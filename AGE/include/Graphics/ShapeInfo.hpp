#pragma once

#include <vector>

#include "Index.hpp"
#include "Vertex.hpp"
#include "Buffer.hpp"

namespace age {

struct ShapeInfo {
    struct {
        // Fixme: this is used only while creating pipeline move it somewhere else
        VkVertexInputBindingDescription bindingDescription;
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions;

        Buffer buffer;
    } vertex;

    struct {
        uint32_t count;
        VkIndexType type;
        Buffer buffer;
    } index;

    template<class T>
    inline ShapeInfo& loadVerticies(const std::vector<Vertex<T>>& verticies) {
        vertex.buffer.create(
            VertexBufferCreateInfo().setSize(verticies.size() * sizeof(T))
        );
        vertex.buffer.loadDeviceLocal(verticies.data(), verticies.size() * sizeof(T));
        Vertex<T>::fillBinding(vertex.bindingDescription);
        Vertex<T>::fillAttributes(vertex.attributeDescriptions);

        return *this;
    }

    inline ShapeInfo& loadIndicies(const std::vector<Index16>& indicies) {
        index.buffer.create(
            IndexBufferCreateInfo().setSize(indicies.size() * sizeof(Index16))
        );
        index.buffer.loadDeviceLocal(indicies.data(), indicies.size() * sizeof(Index16));

        index.type = VK_INDEX_TYPE_UINT16;
        index.count = indicies.size();

        return *this;
    }

    inline ShapeInfo& loadIndicies(const std::vector<Index32>& indicies) {
        index.buffer.create(
            IndexBufferCreateInfo().setSize(indicies.size() * sizeof(Index32))
        );
        index.buffer.loadDeviceLocal(indicies.data(), indicies.size() * sizeof(Index32));

        index.type = VK_INDEX_TYPE_UINT32;
        index.count = indicies.size();

        return *this;
    }

    void destroy() {
        vertex.buffer.destroy();
        index.buffer.destroy();
    }
};

} // namespace age
