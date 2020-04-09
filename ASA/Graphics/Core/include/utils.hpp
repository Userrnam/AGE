#pragma once

#include <vector>

namespace core {

bool checkDeviceExtensionSupport(const VkPhysicalDevice device, const std::vector<const char*>& deviceExtensions);
void getRequeredExtensions(std::vector<const char*>& extensions, bool enableValidationLayers);

// returns number supported features
// returns -1 if device cannot be used
int deviceSupportedFeatures(const VkPhysicalDevice device);

} // namespace core
