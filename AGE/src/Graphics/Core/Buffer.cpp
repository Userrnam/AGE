#include "Buffer.hpp"

#include "Core.hpp"
#include "utils.hpp"
#include "TransitionImageLayout.hpp"

// #include <iostream>

namespace age::core {

void Buffer::destroy() {
	vkDestroyBuffer(apiCore.device, m_buffer, nullptr);
    vkFreeMemory(apiCore.device, m_memory, nullptr);
}

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

// FIXME: add support to load mipmaped images
void Buffer::copyTo(Image& image, VkDeviceSize srcOffset) {
	TransitionInfo transitionInfo;
	transitionInfo.image = image.getImage();
    transitionInfo.mipLevel = image.getMipLevel();
	transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	transitionInfo.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    transitionInfo.srcAccessMask = 0;
	transitionInfo.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	transitionInfo.srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
	transitionInfo.dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	transitionImageLayout(transitionInfo);

	VkCommandBuffer commandBuffer = beginSingleTimeCommands();

	VkBufferImageCopy region = {};
	region.bufferOffset = srcOffset;
	region.bufferImageHeight = 0;
	region.bufferRowLength = 0;
	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;
	region.imageOffset = { 0, 0, 0 };
	region.imageExtent = {
		image.getWidth(),
		image.getHeight(),
		1
	};

	vkCmdCopyBufferToImage(
		commandBuffer,
		m_buffer,
		image.getImage(),
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		1,
		&region
	);

	endSingleTimeCommands(commandBuffer);
}

void Buffer::loadData(const void* data, VkDeviceSize size, VkDeviceSize offset) {
	void* mapped;
	vkMapMemory(apiCore.device, m_memory, offset, size, 0, &mapped);
		memcpy(mapped, data, static_cast<size_t>(size));
	vkUnmapMemory(apiCore.device, m_memory);
}

Buffer createDeviceLocalBuffer(void* data, VkDeviceSize size, VkBufferUsageFlags usage) {
	Buffer out;
	Buffer stagingBuffer;

	BufferCreateInfo createInfo;
	createInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
	createInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	createInfo.size = size;
	stagingBuffer.create(createInfo);

	void* mapped;
	vkMapMemory(apiCore.device, stagingBuffer.getMemory(), 0, size, 0, &mapped);
		memcpy(mapped, data, (size_t)size);
	vkUnmapMemory(apiCore.device, stagingBuffer.getMemory());

	createInfo.memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	createInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage;
	out.create(createInfo);

 // for some reason this throws in debugger
	stagingBuffer.copyTo(out, size);

	stagingBuffer.destroy();

	return out;
}

} // namespace age::core
