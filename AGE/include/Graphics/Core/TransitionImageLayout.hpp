#pragma once

#include <vulkan/vulkan.h>

namespace age::core {

struct TransitionInfo {
    VkImage image;
    uint32_t mipLevel;
    VkImageLayout oldLayout;
    VkImageLayout newLayout;
    VkAccessFlags srcAccessMask;
    VkAccessFlags dstAccessMask;
    VkPipelineStageFlags srcStage;
    VkPipelineStageFlags dstStage;
};

void transitionImageLayout(const TransitionInfo& info);

} // namespace age::core
