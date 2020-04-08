#pragma once

#include <vector>

#include "DeviceRequirements.hpp"

namespace core {

bool checkDeviceExtensionSupport(const VkPhysicalDevice device, const std::vector<const char*>& deviceExtensions);
void getRequeredExtensions(std::vector<const char*>& extensions, bool enableValidationLayers);
bool isDeviceSuitable(const VkPhysicalDevice device, const DeviceRequirements& requirements);

} // namespace core
