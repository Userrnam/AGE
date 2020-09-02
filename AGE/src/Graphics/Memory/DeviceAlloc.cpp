#include <vector>
#include <assert.h>

#include "DeviceAlloc.hpp"
#include "BufferPool.hpp"
#include "Core/Core.hpp"

namespace age::core::deviceAlloc {

// TODO: maybe make multiple buffers for multiple types

BufferPool pool;

MemoryId allocBuffer(uint32_t size, VkBufferUsageFlags usage) {
    VkDeviceSize alignent = 1;

    if (usage & VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT) {
        alignent = apiCore.deviceProperties.limits.minUniformBufferOffsetAlignment;
    }
    if (usage & VK_BUFFER_USAGE_STORAGE_BUFFER_BIT) {
        alignent = std::max(apiCore.deviceProperties.limits.minStorageBufferOffsetAlignment, alignent);
    }
    if (usage & VK_BUFFER_USAGE_TRANSFER_SRC_BIT) {
        alignent = std::max(apiCore.deviceProperties.limits.optimalBufferCopyOffsetAlignment, alignent);
    }

    return pool.allocBuffer(size, alignent);
}

void freeBuffer(MemoryId memory) {
    pool.freeBuffer(memory);
}

void init() {
    pool.create(128 * 1024 * 1024,
    VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT |
    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
    VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
}

void destroy() {
    pool.destroy();
}

} // namespace age::core
