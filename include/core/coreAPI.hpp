#pragma once

#include "DeviceRequirements.hpp"

namespace core {

void debugEnable(bool);
void init(const char *appName, uint32_t appVersion);
void pickPhysicalDevice(DeviceRequirements& requirements);
void destroy();

} // namespace core
