#pragma once

#include <vector>

#include "Index.hpp"
#include "Vertex.hpp"
#include "Buffer.hpp"
#include "viData.hpp"

namespace age {

// contains vertex and index buffers on gpu
struct VIBuffers {
    viVertex vertex;
    viIndex index;

    template<class T>
    inline VIBuffers& loadVerticies(const std::vector<Vertex<T>>& verticies) {
        vertex.buffer.create(
            VertexBufferCreateInfo().setSize(verticies.size() * sizeof(T))
        );
        vertex.buffer.loadDeviceLocal(verticies.data(), verticies.size() * sizeof(T));
        Vertex<T>::fillBinding(vertex.bindingDescription);
        Vertex<T>::fillAttributes(vertex.attributeDescriptions);

        return *this;
    }

    inline VIBuffers& loadIndicies(const std::vector<Index16>& indicies) {
        index.buffer.create(
            IndexBufferCreateInfo().setSize(indicies.size() * sizeof(Index16))
        );
        index.buffer.loadDeviceLocal(indicies.data(), indicies.size() * sizeof(Index16));

        index.type = VK_INDEX_TYPE_UINT16;
        index.count = indicies.size();

        return *this;
    }
};

} // namespace age
