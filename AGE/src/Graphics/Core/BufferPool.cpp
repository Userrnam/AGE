#include "BufferPool.hpp"

#include "Core.hpp"
#include "utils.hpp"

namespace age::core {

void MemoryBlock::create(uint32_t pageSize, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryFlags) {
    m_allocs.push_back({
        0,
        true,
        pageSize
    });

    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = pageSize;
    bufferInfo.usage = usageFlags;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(core::apiCore.device, &bufferInfo, nullptr, &m_buffer) != VK_SUCCESS) {
        throw std::runtime_error("[DeviceAlloc]: failed to create buffer");
    }

    VkMemoryRequirements requirements;
    vkGetBufferMemoryRequirements(apiCore.device, m_buffer, &requirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = pageSize;
    allocInfo.memoryTypeIndex = findMemoryType(requirements.memoryTypeBits, memoryFlags);

    if (vkAllocateMemory(apiCore.device, &allocInfo, nullptr, &m_memory) != VK_SUCCESS) {
        throw std::runtime_error("[DeviceAlloc]: failed to allocate memory");
    }

    vkBindBufferMemory(apiCore.device, m_buffer, m_memory, 0);
}

void MemoryBlock::destroy() {
    vkDestroyBuffer(apiCore.device, m_buffer, nullptr);
    vkFreeMemory(apiCore.device, m_memory, nullptr);
}

AllocAddress MemoryBlock::getAllocAddress(uint32_t size, uint32_t alignment) {
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

void MemoryBlock::free(AllocAddress address) {
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

MemoryId BufferPool::allocBuffer(uint32_t size, uint32_t alignment) {
    assert(size <= m_pageSize);

    MemoryId memoryId;

    for (auto& block : m_memoryBlocks) {
        memoryId.address = block.getAllocAddress(size, alignment);
        if (memoryId.address != -1) {
            memoryId.buffer = block.getBuffer();
            memoryId.memory = block.getMemory();

            return memoryId;
        }
    }

    m_memoryBlocks.push_back(MemoryBlock());
    m_memoryBlocks.back().create(m_pageSize, m_usage, m_memoryFlags);

    memoryId.address = m_memoryBlocks.back().getAllocAddress(size, alignment);
    assert(memoryId.address != -1);

    memoryId.buffer = m_memoryBlocks.back().getBuffer();
    memoryId.memory = m_memoryBlocks.back().getMemory();

    return memoryId;
}

void BufferPool::freeBuffer(MemoryId memory) {
    for (auto& block : m_memoryBlocks) {
        if (block.getBuffer() == memory.buffer) {
            block.free(memory.address);
        }
    }
}

void BufferPool::destroy() {
    for (auto block: m_memoryBlocks) {
        block.destroy();
    }
}

} // namespace age::core