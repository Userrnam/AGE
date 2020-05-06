#include "RenderPassManager.hpp"

#include "Core.hpp"


namespace age::core {

void createFramebuffers(RenderPassRef& renderPassRef) {
    renderPassRef.framebuffers.resize(apiCore.swapchain.imageViews.size());

	size_t swapchainImageViewIndex = 0;
    std::vector<VkImageView> attachments;

    if (renderPassRef.config & RENDER_PASS_DEPTH_BIT) {
        attachments.push_back(apiCore.depth.image.getView());
		swapchainImageViewIndex++;
    }

    if (renderPassRef.config & RENDER_PASS_MULTISAMPLING_BIT) {
        attachments.push_back(apiCore.multisampling.image.getView());
		swapchainImageViewIndex++;
    }

	attachments.push_back(apiCore.swapchain.imageViews[0]);

	for (size_t i = 0; i < apiCore.swapchain.imageViews.size(); ++i) {
		attachments[swapchainImageViewIndex] = apiCore.swapchain.imageViews[i];

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPassRef.renderPass;
		framebufferInfo.attachmentCount = attachments.size();
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = apiCore.swapchain.extent.width;
		framebufferInfo.height = apiCore.swapchain.extent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(apiCore.device, &framebufferInfo, nullptr, &renderPassRef.framebuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create framebuffer");
		}
	}
}

RenderPassRef* createRenderPass(RenderPassConfig rpc) {
    RenderPassRef renderPass = {};
    renderPass.config = rpc;

    std::vector<VkAttachmentDescription> attachments;

    if (rpc & RENDER_PASS_DEPTH_BIT) {
        VkAttachmentDescription depthAttachment = {};
        depthAttachment.format = apiCore.depth.format;
        depthAttachment.samples = apiCore.multisampling.sampleCount;
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        attachments.push_back(depthAttachment);
    }   

    VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = apiCore.swapchain.format;
	if (rpc & RENDER_PASS_MULTISAMPLING_BIT) {
		colorAttachment.samples = apiCore.multisampling.sampleCount;
	} else {
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	}
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	if (rpc & RENDER_PASS_MULTISAMPLING_BIT) {
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	} else {
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	}

    attachments.push_back(colorAttachment);

	
    if (rpc & RENDER_PASS_MULTISAMPLING_BIT) {
        VkAttachmentDescription colorAttachmentResolve = {};
        colorAttachmentResolve.format = apiCore.swapchain.format;
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
    VkAttachmentReference depthAttachmentRef = {};
    VkAttachmentReference colorAttachmentResolveRef = {};
    uint32_t attachmentIndex = 0;

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;

    if (rpc & RENDER_PASS_DEPTH_BIT) {
        depthAttachmentRef.attachment = attachmentIndex;
        depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        subpass.pDepthStencilAttachment = &depthAttachmentRef;
        attachmentIndex++;
    }

	colorAttachmentRef.attachment = attachmentIndex;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	subpass.pColorAttachments = &colorAttachmentRef;
    attachmentIndex++;

    if (rpc & RENDER_PASS_MULTISAMPLING_BIT) {
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

	if (vkCreateRenderPass(apiCore.device, &renderPassInfo, nullptr, &renderPass.renderPass) != VK_SUCCESS) {
		throw std::runtime_error("failed to create render pass");
	}

	createFramebuffers(renderPass);

    apiCore.renderPasses.push_back(renderPass);

    return &apiCore.renderPasses.back();
}

RenderPassRef* requestRenderPass(RenderPassConfig rpc) {
	for (auto& renderPass : apiCore.renderPasses) {
		if (renderPass.config == rpc) {
			return &renderPass;
		}
	}
	
	return createRenderPass(rpc);
}

} // namespace age::core
