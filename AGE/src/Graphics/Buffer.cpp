#include "Buffer.hpp"

#include "Core/Core.hpp"
#include "utils.hpp"
#include "TransitionImageLayout.hpp"
#include "Debug.hpp"

namespace age {

void Buffer::destroy() {
	vkDestroyBuffer(core::apiCore.device, m_buffer, nullptr);
    vkFreeMemory(core::apiCore.device, m_memory, nullptr);
}

void Buffer::create(const BufferCreateInfo& info) {
	ASSERT(info.m_size, "Buffer::create: buffer size must be grater than 0")

	m_size = info.m_size;

    VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = info.m_size;
	bufferInfo.usage = info.m_usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(core::apiCore.device, &bufferInfo, nullptr, &m_buffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to create vertex buffer");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(core::apiCore.device, m_buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	// TODO: store memory type indicies in core or smth to reduce host - device calls
	allocInfo.memoryTypeIndex = core::findMemoryType(memRequirements.memoryTypeBits, info.m_memoryProperties);

	if (vkAllocateMemory(core::apiCore.device, &allocInfo, nullptr, &m_memory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate vertex buffer memory");
	}

	vkBindBufferMemory(core::apiCore.device, m_buffer, m_memory, 0);
}

void Buffer::copyTo(Buffer& buffer, VkDeviceSize size, VkDeviceSize srcOffset, VkDeviceSize dstOffset) {
	VkCommandBuffer commandBuffer = core::beginSingleTimeCommands();

	VkBufferCopy copyRegion = {};
	copyRegion.size = size;
	copyRegion.dstOffset = dstOffset;
	copyRegion.srcOffset = srcOffset;
	vkCmdCopyBuffer(commandBuffer, m_buffer, buffer.m_buffer, 1, &copyRegion);

	core::endSingleTimeCommands(commandBuffer);
}

// FIXME: add support to load mipmaped images
void Buffer::copyTo(core::Image& image, VkDeviceSize srcOffset) {
	core::TransitionInfo transitionInfo;
	transitionInfo.image = image.getImage();
    transitionInfo.mipLevel = image.getMipLevel();
	transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	transitionInfo.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    transitionInfo.srcAccessMask = 0;
	transitionInfo.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	transitionInfo.srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
	transitionInfo.dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	core::transitionImageLayout(transitionInfo);

	VkCommandBuffer commandBuffer = core::beginSingleTimeCommands();

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

	core::endSingleTimeCommands(commandBuffer);
}

void Buffer::load(const void* data, VkDeviceSize size, VkDeviceSize offset) {
	void* mapped;
	vkMapMemory(core::apiCore.device, m_memory, offset, size, 0, &mapped);
		memcpy(mapped, data, static_cast<size_t>(size));
	vkUnmapMemory(core::apiCore.device, m_memory);
}

void Buffer::loadDeviceLocal(const void* data, VkDeviceSize size, VkDeviceSize offset) {
	Buffer stagingBuffer;
	
	stagingBuffer.create(
		BufferCreateInfo()
			.setMemoryProperties(VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
			.setUsage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT)
			.setSize(size)
	);

	stagingBuffer.load(data, size);

	stagingBuffer.copyTo(*this, size, offset);

	stagingBuffer.destroy();
}

} // namespace age
