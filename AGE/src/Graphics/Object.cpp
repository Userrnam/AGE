#include <vulkan/vulkan.h>

#include "Core.hpp"

#include "Object.hpp"
#include "Vertex.hpp"


namespace age {

/*

Pipeline requires:
1. Shaders
2. binding descriptions
3. depth test
4. viewport
5. minSampleRateShading (if multisampling enabled)
6. color blending
7. pipeline layout -> descriptorSetLayouts
8. render pass

NOTE:
shader modules are not required after pipeline is created

*/

void Object::create(ObjectCreateInfo& info) {
    std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

    int i = 0;
    for (auto& shader : info.shaders) {
        shaderStages[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[i].module = reinterpret_cast<VkShaderModule>(shader.getModule());
        shaderStages[i].pName = shader.entryPoint.c_str();
        switch (shader.getType()) {
        case ShaderType::VERTEX_SHADER:
            shaderStages[i].stage = VK_SHADER_STAGE_VERTEX_BIT;
            break;
        
        case ShaderType::FRAGMENT_SHADER:
            shaderStages[i].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
            break;
        }
        i++;
    }

    // auto bindingDescription = Vertex::getBindingDescription();
    // auto attributeDescriptions = Vertex::getAttributeDescriptions();

    // VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo = {};
    // vertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    // vertexInputCreateInfo.vertexBindingDescriptionCount = 1;
    // vertexInputCreateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    // vertexInputCreateInfo.pVertexBindingDescriptions = &bindingDescription;
    // vertexInputCreateInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkPipelineDepthStencilStateCreateInfo depthStencil = {};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.minDepthBounds = 0.0f;
    depthStencil.maxDepthBounds = 1.0f;

    VkViewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)core::apiCore.swapchain.extent.width;
    viewport.height = (float)core::apiCore.swapchain.extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissors = {};
    scissors.offset = { 0, 0 };
    scissors.extent = core::apiCore.swapchain.extent;

    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissors;

    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f;
    rasterizer.depthBiasClamp = 0.0f;
    rasterizer.depthBiasSlopeFactor = 0.0f;

    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_TRUE;
    multisampling.rasterizationSamples = core::apiCore.multisampling.sampleCount;
    multisampling.minSampleShading = 0.2f;

    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                                          VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

    colorBlendAttachment.blendEnable = VK_FALSE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;

    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;

    // VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    // pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    // pipelineLayoutInfo.setLayoutCount = 1;
    // pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
    // pipelineLayoutInfo.pushConstantRangeCount = 0;
    // pipelineLayoutInfo.pPushConstantRanges = nullptr;

    // if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
    //     std::runtime_error("failed to create pipeline layout");
    // }

    VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.stageCount = shaderStages.size();
    pipelineCreateInfo.pStages = shaderStages.data();

    // pipelineCreateInfo.pVertexInputState = &vertexInputCreateInfo;
    pipelineCreateInfo.pInputAssemblyState = &inputAssembly;
    pipelineCreateInfo.pViewportState = &viewportState;
    pipelineCreateInfo.pRasterizationState = &rasterizer;
    pipelineCreateInfo.pMultisampleState = &multisampling;
    pipelineCreateInfo.pDepthStencilState = &depthStencil;
    pipelineCreateInfo.pColorBlendState = &colorBlending;
    pipelineCreateInfo.pDynamicState = nullptr;
    pipelineCreateInfo.pDepthStencilState = &depthStencil;

    // pipelineCreateInfo.layout = pipelineLayout;

    pipelineCreateInfo.renderPass = core::apiCore.renderPass;
    pipelineCreateInfo.subpass = 0; // index

    pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineCreateInfo.basePipelineIndex = -1;


    if (vkCreateGraphicsPipelines(core::apiCore.device, VK_NULL_HANDLE, 1, &pipelineCreateInfo,
    nullptr, reinterpret_cast<VkPipeline*>(&pipeline)) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline");
    }

    // vkDestroyShaderModule(device, vertShaderModule, nullptr);
    // vkDestroyShaderModule(device, fragShaderModule, nullptr);
}

void Object::draw(int i) {
    VkRenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = core::apiCore.renderPass;
    renderPassInfo.framebuffer = core::apiCore.swapchain.framebuffers[i];
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = core::apiCore.swapchain.extent;

    VkBuffer vertexBuffers[] = { (VkBuffer)vertexBuffer };
    VkDeviceSize offsets[] = { 0 };

    vkCmdBeginRenderPass(core::apiCore.commandBuffers.active[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(core::apiCore.commandBuffers.active[i], VK_PIPELINE_BIND_POINT_GRAPHICS, (VkPipeline)pipeline);
        vkCmdBindVertexBuffers(core::apiCore.commandBuffers.active[i], 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(core::apiCore.commandBuffers.active[i], (VkBuffer)indexBuffer, 0, VK_INDEX_TYPE_UINT32);
        // vkCmdBindDescriptorSets(core::apiCore.commandBuffers.active[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[i], 0, nullptr);
        vkCmdDrawIndexed(core::apiCore.commandBuffers.active[i], indexCount, 1, 0, 0, 0);
    vkCmdEndRenderPass(core::apiCore.commandBuffers.active[i]);

    if (vkEndCommandBuffer(core::apiCore.commandBuffers.active[i]) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer");
    }
}

}
