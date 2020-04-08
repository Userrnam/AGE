#pragma once

#include <optional>
#include <vulkan/vulkan.h>

namespace core {

struct QueueFamilyIndicies {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

QueueFamilyIndicies findQueueFamilies(VkPhysicalDevice device);

} // namespace core
