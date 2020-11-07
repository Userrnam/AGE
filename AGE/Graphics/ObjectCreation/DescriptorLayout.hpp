#pragma once

#include <vector>
#include <vulkan/vulkan.h>

namespace age {

// Warning: 
// stage all will not fit
struct Info {
    uint32_t type;
    uint16_t stage;
    uint16_t count;
};

union DescriptorLayoutInfo {
    Info info;
    uint64_t description;
};

struct DescriptorLayout {
    VkDescriptorSetLayout layout;
    std::vector<DescriptorLayoutInfo> layoutInfo;
};
    
} // namespace age

