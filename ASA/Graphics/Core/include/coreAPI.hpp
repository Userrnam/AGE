#pragma once

#include "CoreConfig.hpp"

namespace core {

void debugEnable(bool);
void setCoreConfig(const CoreConfig& config);
void init(const char *appName, uint32_t appVersion);
void pickPhysicalDevice();
void createLogicalDevice();
void createSwapchain();
void createRenderPass();
void createDepthResources();
void destroy();

} // namespace core
