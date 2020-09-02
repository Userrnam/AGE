#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <assert.h>

#include "ShapeManager.hpp"
#include "MemoryHolders/Buffer.hpp"
#include "Memory/BufferPool.hpp"

/*

ShapeManager uses its own pool for buffer allocations

*/

namespace age {

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

core::BufferPool shapeManagerBufferPool;
std::vector<std::pair<ShapeRenderInfo, ShapePipelineCreateDescription>> shapesMap;

void Shape::createManager() {
    // TODO: put page size to core settings
    // TODO: maybe use different pools for vertex and index buffer?
    shapeManagerBufferPool.create(16 * 1024 * 1024,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );

    // init basic shapes here
    // create rectangle shape
    Shape::create(
        ShapeCreateInfo()
        .loadVerticies(verticies)
        .loadIndicies(indicies)
    );
}

void Shape::destroyManager() {
    shapeManagerBufferPool.destroy();
}

ShapeId Shape::create(const ShapeCreateInfo& info) {
    shapesMap.push_back(std::pair<ShapeRenderInfo, ShapePipelineCreateDescription>());
    auto& shape = shapesMap.back();

    // save vertex info
    shape.second = info.m_vertex.description;

    // save index data
    shape.first.m_indexType = info.m_index.type;
    shape.first.m_indexCount = info.m_index.count;

    // load data to buffers
    const std::vector<uint8_t> *datas[2] = {
        &info.m_vertex.data,
        &info.m_index.data
    };
    core::MemoryId *memoryIds[] = {
        &shape.first.m_vertexMemoryId,
        &shape.first.m_indexMemoryId
    };
    for (int i = 0; i < 2; ++i) {
        Buffer stagingBuffer;

        stagingBuffer.create(
            BufferCreateInfo()
            .setUsage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT)
            .setSize(datas[i]->size())
        );

        stagingBuffer.load(datas[i]->data(), datas[i]->size());
        *memoryIds[i] = shapeManagerBufferPool.allocBuffer(datas[i]->size(), 4);
        stagingBuffer.copyTo(memoryIds[i]->buffer, memoryIds[i]->address);
        stagingBuffer.destroy();
    }

    return shapesMap.size() - 1;
}

ShapeRenderInfo Shape::get(ShapeId id) {
    return shapesMap[id].first;
}

const ShapePipelineCreateDescription& Shape::getPipelineCreateDescription(ShapeId id) {
    return shapesMap[id].second;
}

} // namespace age
