#pragma once

#include <vector>

#include "CoreConfig.hpp"
#include "Color.hpp"

namespace age::core {

void debugEnable(bool);
void setCoreConfig(const CoreConfig& config);
void init();
void pickPhysicalDevice();
void createLogicalDevice();
void createSwapchain();
void createDepthResources();
void createMultisamplingResources();
void createCommandPools();

void createSyncObjects();
void allocateCommandBuffers();

// void createCamera();

void setClearColor(const Color& color);

void destroy();

} // namespace age::core
