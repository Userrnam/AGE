#include <vector>

#include "QueueFamilyIndicies.hpp"
#include "Core.hpp"


namespace core {

extern Core apiCore;

// FIXME: add required queues
QueueFamilyIndicies findQueueFamilies(VkPhysicalDevice device) {
	QueueFamilyIndicies indicies;
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamiles(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamiles.data());
	int i = 0;
	for (const auto& queueFamily : queueFamiles) {
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indicies.graphicsFamily = i;
		}
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, apiCore.window.surface, &presentSupport);
		if (presentSupport) {
			indicies.presentFamily = i;
		}
		if (indicies.isComplete()) {
			break;
		}
		i++;
	}
	return indicies;
}

} // namespace core
