#pragma once

#include <vector>

#include "CoreConfig.hpp"
#include "Color.hpp"

namespace age::core {

enum DescriptorUsage {
    UBO_BIT = 1,
    SAMPLER_BIT = 2
};

void debugEnable(bool);
void setCoreConfig(const CoreConfig& config);
void init();
void pickPhysicalDevice();
void createLogicalDevice();
void createSwapchain();
void createRenderPass();
void createDepthResources();
void createMultisamplingResources();
void createFramebuffers();
void createCommandPools();

void createDescriptorPool(DescriptorUsage usage = static_cast<DescriptorUsage>(
    DescriptorUsage::UBO_BIT | DescriptorUsage::SAMPLER_BIT));
void createDescriptorSetLayout(uint32_t uboCount = 1, uint32_t samplerCount = 1);

void createSyncObjects();
void allocateCommandBuffers();

void createCamera();

void setClearColor(const Color& color);

void destroy();

} // namespace age::core
