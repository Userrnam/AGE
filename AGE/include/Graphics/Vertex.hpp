#pragma once

#include <vector>
#include <stdint.h>

#include "Format.hpp"

#define VERTEX_ATTRIBUTES(type) \
    template<> std::vector<VertexAttribute> Vertex<type>::attributes

namespace age {

struct VertexAttribute {
    Format format;
    VertexAttribute(Format f) : format(f) {}
};

namespace __ {

void fillVertexBindingDescription(uint32_t vertexSize, void* description);
void fillAttributeDescriptions(std::vector<VertexAttribute>& attributes, void* pDescriptions);

} // namespace __

template <typename T>
struct Vertex {
    T data;
    static std::vector<VertexAttribute> attributes;

    static inline uint32_t getSize() {
        return sizeof(T);
    }

    static inline void fillBinding(void* description) {
        __::fillVertexBindingDescription(sizeof(T), description);
    }

    static inline void fillAttributes(void* pDescriptions) {
        __::fillAttributeDescriptions(attributes, pDescriptions);
    }
};

} // namespace age
