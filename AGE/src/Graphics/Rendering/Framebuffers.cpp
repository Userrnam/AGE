#include "Framebuffers.hpp"
#include "Core/Core.hpp"

namespace age {

std::vector<VkFramebuffer> Framebuffers::m_framebuffers;

void Framebuffers::create(VkRenderPass renderPass) {
    m_framebuffers.resize(core::apiCore.swapchain.imageViews.size());

	size_t swapchainImageViewIndex = 0;
    std::vector<VkImageView> attachments;

    if (core::apiCore.multisampling.sampleCount != VK_SAMPLE_COUNT_1_BIT) {
        attachments.push_back(core::apiCore.multisampling.image.getView());
		swapchainImageViewIndex++;
    }

	attachments.push_back(core::apiCore.swapchain.imageViews[0]);

	for (size_t i = 0; i < core::apiCore.swapchain.imageViews.size(); ++i) {
		attachments[swapchainImageViewIndex] = core::apiCore.swapchain.imageViews[i];

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = attachments.size();
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = core::apiCore.swapchain.extent.width;
		framebufferInfo.height = core::apiCore.swapchain.extent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(core::apiCore.device, &framebufferInfo, nullptr, &m_framebuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create framebuffer");
		}
	}
}

void Framebuffers::destroy() {
	for (auto framebuffer : m_framebuffers) {
		vkDestroyFramebuffer(core::apiCore.device, framebuffer, nullptr);
	}
}

} // namespace age
