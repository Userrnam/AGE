#pragma once

#include "Buffer.hpp"
#include "Vertex.hpp"

namespace age {

struct viVertex {
    VkVertexInputBindingDescription bindingDescription;
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
    Buffer buffer;
};

struct viIndex {
    uint32_t count;
    VkIndexType type;
    Buffer buffer;
};

} // namespace age
