#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <unordered_map>
#include <assert.h>

#include "ShapeManager.hpp"

namespace age {

Shared<ShapeInfo> rectangle;

static std::vector<Vertex<glm::vec2>> verticies = {
    glm::vec2({ 0.0, 0.0 }),
    glm::vec2({ 1.0, 0.0 }),
    glm::vec2({ 1.0, 1.0 }),
    glm::vec2({ 0.0, 1.0 }),
};

VERTEX_ATTRIBUTES(glm::vec2) = {
    VK_FORMAT_R32G32_SFLOAT
};

static std::vector<Index16> indicies = { 0, 1, 2, 2, 3, 0 };

void initBasicShapes() {
    rectangle.create(
        ShapeInfo()
        .loadIndicies(indicies)
        .loadVerticies(verticies)
    );
}

void destroyBasicShapes() {
    rectangle.destroy();
}

Shared<ShapeInfo> getRectangleShape() {
    return rectangle;
}

uint32_t idCounter = 1;
std::unordered_map<uint32_t, ShapeRenderInfo> shapesMap;

void Shape::createManager() {
    // init basic shapes here
}

void Shape::destroyManager() {
    // maybe create here different 'Buffer Pool for Vertex and Index Buffers'
    // and buffers here are always local
}

ShapeId Shape::create(const ShapeCreateInfo& info) {
    

    ShapeId id;
    id.m_id = idCounter;
    idCounter++;
    return id;
}

ShapeRenderInfo Shape::get(ShapeId id) {
    assert(shapesMap.find(id.m_id) != shapesMap.end());
    // return shapesMap[id];
}


} // namespace age
