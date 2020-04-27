#include "Image.hpp"

#include "Core.hpp"
#include "utils.hpp"

namespace age::core {

void Image::create(ImageCreateInfo& info) {
	VkImageCreateInfo imageInfo = {};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = info.extent.width;
	imageInfo.extent.height = info.extent.height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = info.mipLevel;
	imageInfo.arrayLayers = 1;
	imageInfo.format = info.format;
	imageInfo.tiling = info.tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = info.imageUsage;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imageInfo.samples = info.numberOfSamples;
	imageInfo.flags = 0;

	if (vkCreateImage(apiCore.device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
		throw std::runtime_error("failed to create image");
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(apiCore.device, image, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, info.memoryProperties);

	if (vkAllocateMemory(apiCore.device, &allocInfo, nullptr, &memory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate image memory");
	}

	vkBindImageMemory(apiCore.device, image, memory, 0);

	VkImageAspectFlags aspectFlags = 0;

	imageView = createImageView(image, info.format, info.aspectFlags, info.mipLevel);
}

void Image::destroy() {
	vkDestroyImage(apiCore.device, image, nullptr);
	vkDestroyImageView(apiCore.device, imageView, nullptr);
	vkFreeMemory(apiCore.device, memory, nullptr);
}
    
} // namespace age::core

