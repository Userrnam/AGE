#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace age::core {

struct Pool {
    VkDescriptorPool pool;
    VkDescriptorSetLayout layout;
    std::vector<VkDescriptorSet> sets;
    uint32_t remainingSize;
};
    
} // namespace age::core

