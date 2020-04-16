#pragma once

#include "CoreConfig.hpp"

namespace core {

enum DescriptorUsage {
    UBO_BIT = 1,
    SAMPLER_BIT = 2
};

void debugEnable(bool);
void setCoreConfig(const CoreConfig& config);
void init(const char *appName, uint32_t appVersion);
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
void destroy();

} // namespace core
