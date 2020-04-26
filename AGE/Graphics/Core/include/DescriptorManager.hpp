#pragma once

#include <vulkan/vulkan.h>

namespace age::core {

enum DescriptorUsage {
    UBO_BIT = 1,
    SAMPLER_BIT = 2
};

VkDescriptorPool requestDescriptorPool(DescriptorUsage usage);
VkDescriptorSetLayout requestDescriptorSetLayout(uint32_t uboCount, uint32_t samplerCount);

VkDescriptorPool createDescriptorPool(DescriptorUsage usage);
VkDescriptorSetLayout createDescriptorSetLayout(uint32_t uboCount, uint32_t samplerCount);
    
} // namespace age::core
