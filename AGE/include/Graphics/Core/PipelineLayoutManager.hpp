#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace age::core {

VkPipelineLayout requestPipelineLayout(const std::vector<VkDescriptorSetLayout>& layouts);
    
} // namespace age::core
