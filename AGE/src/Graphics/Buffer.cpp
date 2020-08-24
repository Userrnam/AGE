#include "Buffer.hpp"

#include "Core/Core.hpp"
#include "utils.hpp"
#include "TransitionImageLayout.hpp"
#include "Debug.hpp"

namespace age {

void Buffer::destroy() {
	core::deviceAlloc::freeBuffer(m_memoryId);
}

void Buffer::create(const BufferCreateInfo& info) {
	ASSERT(info.m_size, "Buffer::create: buffer size must be grater than 0")
	m_size = info.m_size;
	m_memoryId = core::deviceAlloc::allocBuffer(info.m_size, info.m_usage);
}

void Buffer::copyTo(core::Image& image, VkDeviceSize srcOffset) {
	core::TransitionInfo transitionInfo;
	transitionInfo.image = image.getImage();
	transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	transitionInfo.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    transitionInfo.srcAccessMask = 0;
	transitionInfo.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	transitionInfo.srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
	transitionInfo.dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	core::transitionImageLayout(transitionInfo);

	VkCommandBuffer commandBuffer = core::beginSingleTimeCommands();

	VkBufferImageCopy region = {};
	region.bufferOffset = srcOffset + m_memoryId.address;
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
		m_memoryId.buffer,
		image.getImage(),
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		1,
		&region
	);

	core::endSingleTimeCommands(commandBuffer);
	
	transitionInfo.image = image.getImage();
	transitionInfo.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	transitionInfo.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    transitionInfo.srcAccessMask = 0;
	transitionInfo.dstAccessMask = 0;
	transitionInfo.srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	transitionInfo.dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	transitionImageLayout(transitionInfo);
}

void Buffer::load(const void* data, VkDeviceSize size, VkDeviceSize offset) {
	void* mapped;
	vkMapMemory(core::apiCore.device, m_memoryId.memory, m_memoryId.address + offset, size, 0, &mapped);
		memcpy(mapped, data, static_cast<size_t>(size));
	vkUnmapMemory(core::apiCore.device, m_memoryId.memory);
}

} // namespace age
