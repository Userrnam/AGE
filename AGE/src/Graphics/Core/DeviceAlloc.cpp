#include <vector>
#include <assert.h>

#include "DeviceAlloc.hpp"
#include "utils.hpp"
#include "Core.hpp"

namespace age::core::deviceAlloc {

// TODO: maybe make multiple buffers for multiple types

typedef uint32_t AllocSize;

constexpr static AllocSize pageSize = 16 * 1024 * 1024;

struct AllocBlock {
    AllocAddress address;
    bool isFree :     1;
    AllocSize size : 31;

    AllocBlock() {}
    inline AllocBlock(AllocAddress _address, bool _isFree, AllocSize _size)
        : address(_address), isFree(_isFree), size(_size) {}
};

class MemoryBlock {
    std::vector<AllocBlock> m_allocs;
    VkBuffer m_buffer;
    VkDeviceMemory m_memory;

public:
    inline VkBuffer getBuffer() {
        return m_buffer;
    }

    MemoryBlock() {}

    void create() {
        m_allocs.push_back({
            0,
            true,
            pageSize
        });

        VkBufferCreateInfo bufferInfo = {};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = pageSize;
        bufferInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(core::apiCore.device, &bufferInfo, nullptr, &m_buffer) != VK_SUCCESS) {
            throw std::runtime_error("[DeviceAlloc]: failed to create buffer");
        }

        VkMemoryRequirements requirements;
        vkGetBufferMemoryRequirements(apiCore.device, m_buffer, &requirements);

        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = pageSize;
        allocInfo.memoryTypeIndex = findMemoryType(requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        if (vkAllocateMemory(apiCore.device, &allocInfo, nullptr, &m_memory) != VK_SUCCESS) {
            throw std::runtime_error("[DeviceAlloc]: failed to allocate memory");
        }

        vkBindBufferMemory(apiCore.device, m_buffer, m_memory, 0);
    }

    void destroy() {
        vkDestroyBuffer(apiCore.device, m_buffer, nullptr);
        vkFreeMemory(apiCore.device, m_memory, nullptr);
    }

    // returns -1 if this block doesn't have enough memory
    AllocAddress getAllocAddress(uint32_t size, uint32_t alignment) {
        uint32_t alignmentExtra;
        for (size_t i = 0; i < m_allocs.size(); ++i) {
            if (m_allocs[i].isFree && m_allocs[i].size >= size + (alignmentExtra = (alignment - m_allocs[i].address % alignment) % alignment)) {
                m_allocs[i].isFree = false;
                m_allocs[i].address += alignmentExtra;
                m_allocs[i].size -= alignmentExtra;
                if (i > 0) {
                    m_allocs[i - 1].size += alignmentExtra;
                }
                if (m_allocs[i].size == size) {
                    return m_allocs[i].address;
                }
                // split AllocBlock
                m_allocs.insert(m_allocs.begin() + i + 1, AllocBlock(m_allocs[i].address + size, true, m_allocs[i].size - size));
                m_allocs[i].size = size;

                return m_allocs[i].address;
            }
        }
        return -1;
    }

    void free(AllocAddress address) {
        for (size_t i = 0; i < m_allocs.size(); ++i) {
            if (m_allocs[i].address == address) {
                m_allocs[i].isFree = true;

                // check if this alloc can be merged with next
                if (i < m_allocs.size() && m_allocs[i + 1].isFree) {
                    m_allocs[i].size += m_allocs[i + 1].size;
                    m_allocs.erase(m_allocs.begin() + i + 1);
                }

                // check if this alloc can be merged with previous
                if (i > 0 && m_allocs[i - 1].isFree) {
                    m_allocs[i - 1].size += m_allocs[i].size;
                    m_allocs.erase(m_allocs.begin() + i);
                }
            }
        }
    }

    inline VkDeviceMemory getMemory() {
        return m_memory;
    }
};

std::vector<MemoryBlock> memoryMap;

MemoryId allocBuffer(uint32_t size, VkBufferUsageFlags usage) {
    assert(size <= pageSize);

    MemoryId memoryId;

    VkDeviceSize alignemnt = 16;
    if (usage & VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT) {
        alignemnt = apiCore.deviceProperties.limits.minUniformBufferOffsetAlignment;
    }
    if (usage & VK_BUFFER_USAGE_STORAGE_BUFFER_BIT) {
        alignemnt = std::max(apiCore.deviceProperties.limits.minStorageBufferOffsetAlignment, alignemnt);
    }
    if (usage & VK_BUFFER_USAGE_TRANSFER_SRC_BIT) {
        alignemnt = std::max(apiCore.deviceProperties.limits.optimalBufferCopyOffsetAlignment, alignemnt);
    }

    for (auto& block : memoryMap) {
        memoryId.address = block.getAllocAddress(size, alignemnt);
        if (memoryId.address != -1) {
            memoryId.buffer = block.getBuffer();
            memoryId.memory = block.getMemory();

            return memoryId;
        }
    }

    memoryMap.push_back(MemoryBlock());
    memoryMap.back().create();

    memoryId.address = memoryMap.back().getAllocAddress(size, alignemnt);
    assert(memoryId.address != -1);

    memoryId.buffer = memoryMap.back().getBuffer();
    memoryId.memory = memoryMap.back().getMemory();

    return memoryId;
}

void freeBuffer(MemoryId memory) {
    for (auto& block : memoryMap) {
        if (block.getBuffer() == memory.buffer) {
            block.free(memory.address);
        }
    }
}

void destroy() {
    for (auto block: memoryMap) {
        block.destroy();
    }
}

} // namespace age::core
