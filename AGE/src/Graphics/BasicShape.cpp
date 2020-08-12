#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include "BasicShape.hpp"

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

} // namespace age
