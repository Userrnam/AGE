#pragma once

#include <vulkan/vulkan.h>

namespace age {

struct DD {
    VkDescriptorSetLayout layout;
    VkDescriptorSet set;
    uint64_t info;
};

} // namespace age
