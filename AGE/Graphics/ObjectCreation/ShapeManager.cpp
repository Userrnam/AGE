#include <vulkan/vulkan.h>
#include "../../Math/Vector.hpp"
#include <assert.h>

#include "ShapeManager.hpp"
#include "../MemoryHolders/Buffer.hpp"
#include "../Memory/BufferPool.hpp"

/*

ShapeManager uses its own pool for buffer allocations

*/

namespace age {

static std::vector<Vector2f> verticies = {
    Vector2f({ 0.0, 0.0 }),
    Vector2f({ 1.0, 0.0 }),
    Vector2f({ 1.0, 1.0 }),
    Vector2f({ 0.0, 1.0 }),
};

static std::vector<Index16> indicies = { 0, 1, 2, 2, 3, 0 };

core::BufferPool shapeManagerBufferPool;
std::vector<ShapeRenderInfo> shapesMap;

void Shape::createManager() {
    // TODO: put page size to core settings
    // TODO: maybe use different pools for vertex and index buffer?
    shapeManagerBufferPool.create(16 * 1024 * 1024,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        false
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
    shapesMap.push_back(ShapeRenderInfo());
    auto& shape = shapesMap.back();

    // save index data
    shape.m_indexCount = info.m_index.count;

    // load data to buffers
    const std::vector<uint8_t> *datas[2] = {
        &info.m_vertex.data,
        &info.m_index.data
    };
    core::MemoryId *memoryIds[] = {
        &shape.m_vertexMemoryId,
        &shape.m_indexMemoryId
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
    return shapesMap[id];
}

} // namespace age
