#include "Buffer.hpp"

#include "Core.hpp"
#include "utils.hpp"

namespace age::core {

void Buffer::create(BufferCreateInfo& info) {
    VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = info.size;
	bufferInfo.usage = info.usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(apiCore.device, &bufferInfo, nullptr, &m_buffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to create vertex buffer");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(apiCore.device, m_buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, info.memoryProperties);

	if (vkAllocateMemory(apiCore.device, &allocInfo, nullptr, &m_memory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate vertex buffer memory");
	}

	vkBindBufferMemory(apiCore.device, m_buffer, m_memory, 0);
}

void Buffer::copyTo(Buffer& buffer, VkDeviceSize size, VkDeviceSize srcOffset, VkDeviceSize dstOffset) {
	VkCommandBuffer commandBuffer = beginSingleTimeCommands();

	VkBufferCopy copyRegion = {};
	copyRegion.size = size;
	copyRegion.dstOffset = dstOffset;
	copyRegion.srcOffset = srcOffset;
	vkCmdCopyBuffer(commandBuffer, m_buffer, buffer.m_buffer, 1, &copyRegion);

	endSingleTimeCommands(commandBuffer);
}

void Buffer::loadData(void* data, VkDeviceSize size) {
	void* mapped;
	vkMapMemory(apiCore.device, m_memory, 0, size, 0, &mapped);
	memcpy(mapped, data, static_cast<size_t>(size));
	vkUnmapMemory(apiCore.device, m_memory);
}

void Buffer::destroy() {
	vkDestroyBuffer(apiCore.device, m_buffer, nullptr);
    vkFreeMemory(apiCore.device, m_memory, nullptr);
}

Buffer createDeviceLocalBuffer(void* data, VkDeviceSize size, VkBufferUsageFlags usage) {
	Buffer out;
	Buffer stagingBuffer;

	BufferCreateInfo createInfo;
	createInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
	createInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	createInfo.size = size;
	stagingBuffer.create(createInfo);

	createInfo.memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	createInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage;
	out.create(createInfo);

	stagingBuffer.copyTo(out, size);

	stagingBuffer.destroy();

	return out;
}

} // namespace age::core
