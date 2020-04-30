#pragma once

#include <vulkan/vulkan.h>

namespace age {

struct DescriptorLayout {
    VkDescriptorSetLayout layout;
    uint32_t uboCount;
    uint32_t uboBinding;
    uint32_t samplerCount;
    uint32_t samplerBinding;
};
    
} // namespace age

