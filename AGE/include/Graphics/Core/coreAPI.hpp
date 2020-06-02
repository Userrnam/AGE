#pragma once

#include <vector>

#include "CoreConfig.hpp"
#include "Color.hpp"

namespace age::core {

// void setCoreConfig(const CoreConfig& config);
void init();
void pickPhysicalDevice();
void createLogicalDevice();
void createSwapchain();
void createDepthResources();
void createMultisamplingResources();
void createCommandPools();

void createSyncObjects();
void allocateCommandBuffers();

} // namespace age::core
