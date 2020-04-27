#include <vulkan/vulkan.h>

#include "Command.hpp"
#include "Core.hpp"


namespace age::core {

namespace cmd {

void clear(int i) {
	VkImageSubresourceRange isr;
	isr.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	isr.baseMipLevel = 0;
	isr.levelCount = 1;
	isr.baseArrayLayer = 0;
	isr.layerCount = 1;

    VkImageMemoryBarrier presentToClearBarrier = {};
    presentToClearBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    presentToClearBarrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
    presentToClearBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    presentToClearBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    presentToClearBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    presentToClearBarrier.srcQueueFamilyIndex = apiCore.queues.graphics.index;
    presentToClearBarrier.dstQueueFamilyIndex = apiCore.queues.graphics.index;
    presentToClearBarrier.image = apiCore.swapchain.images[i];
    presentToClearBarrier.subresourceRange = isr;

    // Change layout of image to be optimal for presenting
    VkImageMemoryBarrier clearToPresentBarrier = {};
    clearToPresentBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    clearToPresentBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    clearToPresentBarrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
    clearToPresentBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    clearToPresentBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    clearToPresentBarrier.srcQueueFamilyIndex = apiCore.queues.graphics.index;
    clearToPresentBarrier.dstQueueFamilyIndex = apiCore.queues.graphics.index;
    clearToPresentBarrier.image = apiCore.swapchain.images[i];
    clearToPresentBarrier.subresourceRange = isr;

    vkCmdPipelineBarrier(apiCore.commandBuffers.active[i], VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &presentToClearBarrier);

    vkCmdClearColorImage(
        apiCore.commandBuffers.active[i],
        apiCore.swapchain.images[i],
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        &apiCore.swapchain.clearColor, 1, &isr
    );

    vkCmdPipelineBarrier(apiCore.commandBuffers.active[i], VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &clearToPresentBarrier);
}

} // namespace cmd
} // namespace age::core
