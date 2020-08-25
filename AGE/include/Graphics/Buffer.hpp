#pragma once

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <cstring>

#include "Core/Image.hpp"
#include "Core/DeviceAlloc.hpp"

namespace age {

class BufferCreateInfo {
    VkDeviceSize m_size;
    VkBufferUsageFlags m_usage;

    friend class Buffer;
public:
    inline BufferCreateInfo& setSize(VkDeviceSize size) {
        m_size = size;
        return *this;
    }

    inline BufferCreateInfo& setUsage(VkBufferUsageFlags usage) {
        m_usage = usage;
        return *this;
    }
};

class VertexBufferCreateInfo {
    BufferCreateInfo createInfo;
public:
    VertexBufferCreateInfo() {
        createInfo.setUsage(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    }

    inline VertexBufferCreateInfo& setSize(VkDeviceSize size) {
        createInfo.setSize(size);
        return *this;
    }

    inline operator BufferCreateInfo() {
        return createInfo;
    }
};

class IndexBufferCreateInfo {
    BufferCreateInfo createInfo;
public:
    IndexBufferCreateInfo() {
        createInfo.setUsage(VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
    }

    inline IndexBufferCreateInfo& setSize(VkDeviceSize size) {
        createInfo.setSize(size);
        return *this;
    }

    inline operator BufferCreateInfo() {
        return createInfo;
    }
};

class UniformBufferCreateInfo {
    BufferCreateInfo createInfo;
public:
    UniformBufferCreateInfo() {
        createInfo.setUsage(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
    }

    inline UniformBufferCreateInfo& setSize(VkDeviceSize size) {
        createInfo.setSize(size);
        return *this;
    }

    operator BufferCreateInfo() {
        return createInfo;
    }
};

class Buffer {
protected:
    VkDeviceSize m_size = VK_NULL_HANDLE;
    core::MemoryId m_memoryId;
public:
    void destroy();

    inline VkBuffer getBuffer() const { return m_memoryId.buffer; }
    inline uint32_t getBufferOffset() const { return m_memoryId.address; }
    inline VkDeviceSize getSize() const { return m_size; }

    void create(const BufferCreateInfo& info);

    void copyTo(core::Image& image, VkDeviceSize srcOffset=0);
    void load(const void* data, VkDeviceSize size, VkDeviceSize offset=0);
};

} // namespace age
