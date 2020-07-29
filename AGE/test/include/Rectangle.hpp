#pragma once

#include "Graphics.hpp"
#include "VIBuffers.hpp"

struct RectangleVertexElement {
    glm::vec2 m_coord;

    RectangleVertexElement(glm::vec2 coord) : m_coord(coord) {}
};

std::vector<age::Vertex<RectangleVertexElement>> verticies = {
    RectangleVertexElement({0, 0}),
    RectangleVertexElement({1, 0}),
    RectangleVertexElement({1, 1}),
    RectangleVertexElement({0, 1}),
};

VERTEX_ATTRIBUTES(RectangleVertexElement) = {
    VK_FORMAT_R32G32_SFLOAT
};

std::vector<age::Index16> indicies = {
    0, 1, 2, 3
};

// VIBuffers - component that can be used by all rectangles for example

struct RectangleVertexBuffer {
    age::VIBuffers viBuffers;
    void dos() {
        viBuffers.loadIndicies(indicies);
    }
};
