#include "Renderer.hpp"
#include "Graphics/Core/Command.hpp"
#include "Graphics/Core/CoreCreator.hpp"
#include "Graphics/Core/Core.hpp"

namespace age {

void Renderer::updateCommandBuffers() {
    // update active commandBuffer
    if (core::apiCore.commandBuffers.active == core::apiCore.commandBuffers.data.data()) {
        core::apiCore.commandBuffers.active += core::apiCore.swapchain.images.size();
    } else {
        core::apiCore.commandBuffers.active -= core::apiCore.swapchain.images.size();
    }

    VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0;
	beginInfo.pInheritanceInfo = nullptr;

    VkRenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = core::apiCore.renderPass.renderPass;
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = core::apiCore.swapchain.extent;

	for (size_t i = 0; i < core::apiCore.commandBuffers.size; ++i) {
        vkResetCommandBuffer(core::apiCore.commandBuffers.active[i], 0);
		if (vkBeginCommandBuffer(core::apiCore.commandBuffers.active[i], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer");
		}

        renderPassInfo.framebuffer = core::apiCore.renderPass.framebuffers[i];

        vkCmdBeginRenderPass(core::apiCore.commandBuffers.active[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        draw(i);

        vkCmdEndRenderPass(core::apiCore.commandBuffers.active[i]);

		if (vkEndCommandBuffer(core::apiCore.commandBuffers.active[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer");
		}
	}
}

void Renderer::draw(int j) {
    for (size_t i = 0; i < pTargets->size(); ++i) {
        (*pTargets)[i].draw(j);
    }
}

void Renderer::render(std::vector<Drawable>& targets) {
    // deside if we need update
    this->pTargets = &targets;
    updateCommandBuffers();
}

void Renderer::init() {
    core::initCore();

    // clear screen
    VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0;
	beginInfo.pInheritanceInfo = nullptr;

	for (size_t i = 0; i < core::apiCore.commandBuffers.size; ++i) {
        vkResetCommandBuffer(core::apiCore.commandBuffers.active[i], 0);
		if (vkBeginCommandBuffer(core::apiCore.commandBuffers.active[i], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer");
		}

        core::cmd::clear(i, {});

		if (vkEndCommandBuffer(core::apiCore.commandBuffers.active[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer");
		}
	}
}

void Renderer::destroy() {
    core::destroyCore();
}

} // namespace age
