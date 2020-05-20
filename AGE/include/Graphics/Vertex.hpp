#pragma once

#include <vector>
#include <stdint.h>
#include <vulkan/vulkan.h>

#include "Format.hpp"

#define VERTEX_ATTRIBUTES(type) \
    template<> std::vector<VertexAttribute> Vertex<type>::attributes

namespace age {

struct VertexAttribute {
    VkFormat format;
    VertexAttribute(VkFormat f) : format(f) {}
};

template <typename T>
struct Vertex {
    T data;
    static std::vector<VertexAttribute> attributes;

    static inline void fillBinding(VkVertexInputBindingDescription& description) {
        description.binding = 0;
        description.stride = sizeof(T);
        description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    }

    static inline void fillAttributes(std::vector<VkVertexInputAttributeDescription>& vDescriptions) {
        vDescriptions.resize(attributes.size());
        auto descriptions = vDescriptions.data();

        size_t descriptionIndex = 0;
        uint32_t offset = 0;
        for (size_t i = 0; i < attributes.size(); ++i) {
            descriptions[descriptionIndex].binding = 0;
            descriptions[descriptionIndex].location = i;
            descriptions[descriptionIndex].offset = offset;
            descriptions[descriptionIndex].format = static_cast<VkFormat>(attributes[i].format);

            descriptionIndex += sizeof(VkVertexInputAttributeDescription);
            offset += getFormatSize(attributes[i].format);
        }
    }
};

} // namespace age
