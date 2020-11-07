#pragma once

#include <vulkan/vulkan.h>
#include <vector>

#include "MemoryId.hpp"

namespace age::core {

typedef uint32_t AllocSize;

struct AllocBlock {
    AllocAddress address;
    bool isFree :     1;
    AllocSize size : 31;

    AllocBlock() {}
    inline AllocBlock(AllocAddress _address, bool _isFree, AllocSize _size)
        : address(_address), isFree(_isFree), size(_size) {}
};

class MemoryPage {
    std::vector<AllocBlock> m_allocs;
    VkBuffer m_buffer;
    VkDeviceMemory m_memory;

public:
    inline VkBuffer getBuffer() const { return m_buffer; }
    inline VkDeviceMemory getMemory() { return m_memory; }

    void create(uint32_t pageSize, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryFlags);
    void destroy();

    // returns -1 if this block doesn't have enough memory
    AllocAddress getAllocAddress(uint32_t size, uint32_t alignment); 

    void free(AllocAddress address);
    void status();
    bool isFree();
};

class BufferPool {
    uint32_t m_pageSize = 0;
    VkBufferUsageFlags m_usage = 0;
    VkMemoryPropertyFlags m_memoryFlags = 0;
    std::vector<MemoryPage> m_memoryPages;
    bool m_checkOnDestroy = 0;
public:
    inline void create(uint32_t pageSize, VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryFlags, bool checkOnDestroy = true) {
        m_pageSize = pageSize;
        m_usage = usage;
        m_memoryFlags = memoryFlags;
        m_checkOnDestroy = checkOnDestroy;
    }

    void destroy();

    MemoryId allocBuffer(uint32_t size, uint32_t alignment);
    void freeBuffer(MemoryId id);
};

} // namespace age::core
