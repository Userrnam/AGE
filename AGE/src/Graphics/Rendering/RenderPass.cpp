#include "RenderPass.hpp"

#include "Core/Core.hpp"

namespace age {

VkRenderPass RenderPass::m_renderPass;

void RenderPass::create() {
    std::vector<VkAttachmentDescription> attachments;

    VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = core::apiCore.swapchain.format;
    colorAttachment.samples = core::apiCore.multisampling.sampleCount;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	if (core::apiCore.multisampling.sampleCount != VK_SAMPLE_COUNT_1_BIT) {
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	} else {
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	}

    attachments.push_back(colorAttachment);

    if (core::apiCore.multisampling.sampleCount != VK_SAMPLE_COUNT_1_BIT) {
        VkAttachmentDescription colorAttachmentResolve = {};
        colorAttachmentResolve.format = core::apiCore.swapchain.format;
        colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        attachments.push_back(colorAttachmentResolve);
    }

    VkAttachmentReference colorAttachmentRef = {};
    VkAttachmentReference colorAttachmentResolveRef = {};
    uint32_t attachmentIndex = 0;

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;

	colorAttachmentRef.attachment = attachmentIndex;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	subpass.pColorAttachments = &colorAttachmentRef;
    attachmentIndex++;

    if (core::apiCore.multisampling.sampleCount != VK_SAMPLE_COUNT_1_BIT) {
        colorAttachmentResolveRef.attachment = attachmentIndex;
        colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        subpass.pResolveAttachments = &colorAttachmentResolveRef;
        attachmentIndex++;
    }

	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = attachments.size();
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	if (vkCreateRenderPass(core::apiCore.device, &renderPassInfo, nullptr, &m_renderPass) != VK_SUCCESS) {
		throw std::runtime_error("failed to create render pass");
	}
}

void RenderPass::destroy() {
	vkDestroyRenderPass(core::apiCore.device, m_renderPass, nullptr);
}


} // namespace age
