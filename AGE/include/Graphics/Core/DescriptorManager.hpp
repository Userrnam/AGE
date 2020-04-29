#pragma once

#include <vulkan/vulkan.h>

namespace age::core {

VkDescriptorPool requestDescriptorPool(uint32_t uboCount, uint32_t samplerCount);
VkDescriptorSetLayout requestDescriptorSetLayout(uint32_t uboCount, uint32_t samplerCount);

VkDescriptorPool createDescriptorPool(uint32_t uboCount, uint32_t samplerCount);
VkDescriptorSetLayout createDescriptorSetLayout(uint32_t uboCount, uint32_t samplerCount);
    
} // namespace age::core
