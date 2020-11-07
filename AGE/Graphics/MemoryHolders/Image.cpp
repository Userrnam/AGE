#include <stdexcept>

#include "Image.hpp"

#include "../Core/Core.hpp"
#include "../Core/Utils/utils.hpp"

namespace age::core {

void Image::create(ImageCreateInfo& info) {
	m_extent = info.m_extent;
	m_format = info.m_format;

	VkImageCreateInfo imageInfo = {};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = m_extent.width;
	imageInfo.extent.height = m_extent.height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = info.m_format;
	imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = info.m_imageUsage;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageInfo.samples = info.m_sampleCount;
	imageInfo.flags = 0;

	if (vkCreateImage(apiCore.device, &imageInfo, nullptr, &m_image) != VK_SUCCESS) {
		throw std::runtime_error("failed to create image");
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(apiCore.device, m_image, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	if (vkAllocateMemory(apiCore.device, &allocInfo, nullptr, &m_memory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate image memory");
	}

	vkBindImageMemory(apiCore.device, m_image, m_memory, 0);

//	VkImageAspectFlags aspectFlags = 0;
	m_imageView = createImageView(m_image, info.m_format, info.m_aspectFlags);
}

void Image::destroy() {
	vkDestroyImage(apiCore.device, m_image, nullptr);
	vkDestroyImageView(apiCore.device, m_imageView, nullptr);
	vkFreeMemory(apiCore.device, m_memory, nullptr);
}
 
} // namespace age::core
