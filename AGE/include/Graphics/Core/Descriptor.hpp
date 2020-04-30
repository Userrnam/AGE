#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace age::core {

struct Descriptor {
    std::vector<VkDescriptorSetLayout> layouts;
    std::vector<VkDescriptorSet> sets;
};

} // namespace age::core