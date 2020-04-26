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

	if (vkCreateBuffer(apiCore.device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to create vertex buffer");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(apiCore.device, buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, info.memoryProperties);

	if (vkAllocateMemory(apiCore.device, &allocInfo, nullptr, &memory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate vertex buffer memory");
	}

	vkBindBufferMemory(apiCore.device, buffer, memory, 0);
}

void Buffer::destroy() {
	vkDestroyBuffer(apiCore.device, buffer, nullptr);
    vkFreeMemory(apiCore.device, memory, nullptr);
}

} // namespace age::core
