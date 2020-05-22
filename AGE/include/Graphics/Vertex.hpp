#pragma once

#include <vector>
#include <stdint.h>
#include <vulkan/vulkan.h>

#include "Format.hpp"

#define VERTEX_ATTRIBUTES(type) \
    template<> std::vector<age::VertexAttribute> age::Vertex<type>::attributes

namespace age {

struct VertexAttribute {
    VkFormat format;
    VertexAttribute(VkFormat f) : format(f) {}
};

template <typename T>
struct Vertex {
    T data;
    static std::vector<VertexAttribute> attributes;

    Vertex(T elem) : data(elem) {}

    static inline void fillBinding(VkVertexInputBindingDescription& description) {
        description.binding = 0;
        description.stride = sizeof(T);
        description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    }

    static inline void fillAttributes(std::vector<VkVertexInputAttributeDescription>& descriptions) {
        descriptions.resize(attributes.size());
        uint32_t offset = 0;
        for (size_t i = 0; i < attributes.size(); ++i) {
            descriptions[i].binding = 0;
            descriptions[i].location = i;
            descriptions[i].offset = offset;
            descriptions[i].format = attributes[i].format;

            offset += getFormatSize(attributes[i].format);
        }
    }
};

} // namespace age
