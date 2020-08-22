#include "Drawable.hpp"

#include "Core/Core.hpp"
#include "CoreConfig.hpp"
#include "Core/RenderPassManager.hpp"
#include "PipelineLayoutManager.hpp"
#include "Core/Pool.hpp"

namespace age::core {
    extern CoreConfig coreConfig;
}

namespace age {

// Fixme: same pipelines can be used with different descriptorSets
// objects that use same shaders, have same descriptors and same vertex type can use same pipeline
void Drawable::createDrawable(const DrawableCreateInfo& info) {
    m_shapeInfo = *info.m_pShapeInfo;
    m_instanceCount = info.m_instanceCount;

    std::vector<VkDescriptorSetLayout> layouts;
    layouts.reserve(info.m_descriptors.size() + 1);
    m_poolIndicies.reserve(info.m_descriptors.size() + 1);
    m_descriptorSets.reserve(info.m_descriptors.size() + 1);

    // add camera descriptor
    auto cameraDescriptor = info.m_view->getDescriptor();
    m_descriptorSets.push_back(cameraDescriptor.m_set);
    m_poolIndicies.push_back(cameraDescriptor.m_poolIndex);
    layouts.push_back(cameraDescriptor.m_layout);

    for (auto& d : info.m_descriptors) {
        m_descriptorSets.push_back(d.m_set);
        m_poolIndicies.push_back(d.m_poolIndex);
        layouts.push_back(d.m_layout);
    }

    std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
    shaderStages.resize(info.m_shaders.size());

    std::vector<VkSpecializationInfo> specializationInfos;
    uint32_t specIndex = 0;
    specializationInfos.resize(info.m_shaders.size());
    for (size_t i = 0; i < info.m_shaders.size(); ++i) {
        shaderStages[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[i].module = info.m_shaders[i].getModule();
        shaderStages[i].pName = info.m_shaders[i].getEntry().c_str();
        shaderStages[i].stage = info.m_shaders[i].getStage();

        if (info.m_shaders[i].getSpecialization().m_data.size() != 0) {
            specializationInfos[specIndex].dataSize = info.m_shaders[i].getSpecialization().m_data.size() * sizeof(uint8_t);
            specializationInfos[specIndex].mapEntryCount = info.m_shaders[i].getSpecialization().m_entries.size();
            specializationInfos[specIndex].pData = info.m_shaders[i].getSpecialization().m_data.data();
            specializationInfos[specIndex].pMapEntries = info.m_shaders[i].getSpecialization().m_entries.data();

            shaderStages[i].pSpecializationInfo = &specializationInfos[specIndex];

            specIndex++;
        }
    }

    // shape info (vertex data) only required here
    VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo = {};
    vertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputCreateInfo.vertexBindingDescriptionCount = 1;
    vertexInputCreateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(info.m_pShapeInfo->data.vertex.attributeDescriptions.size());
    vertexInputCreateInfo.pVertexBindingDescriptions = &info.m_pShapeInfo->data.vertex.bindingDescription;
    vertexInputCreateInfo.pVertexAttributeDescriptions = info.m_pShapeInfo->data.vertex.attributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkPipelineDepthStencilStateCreateInfo depthStencil = {};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;

    VkRect2D scissors = {};
    scissors.offset = { 0, 0 };
    scissors.extent = core::apiCore.swapchain.extent;

    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissors;

    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_NONE;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f;
    rasterizer.depthBiasClamp = 0.0f;
    rasterizer.depthBiasSlopeFactor = 0.0f;

    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    if (core::coreConfig.multisampling.sampleCount != VK_SAMPLE_COUNT_1_BIT) {
        multisampling.rasterizationSamples = core::apiCore.multisampling.sampleCount;
        if (core::coreConfig.multisampling.minSampleShading) {
            multisampling.sampleShadingEnable = VK_TRUE;
            multisampling.minSampleShading = core::coreConfig.multisampling.minSampleShading;
        }
    } else {
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    }

    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                                          VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

    colorBlendAttachment.blendEnable = info.m_colorBlending;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;

    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;

    m_pipelineLayout = core::requestPipelineLayout(layouts);

    VkDynamicState dynamicStates[] = { VK_DYNAMIC_STATE_VIEWPORT };
    VkPipelineDynamicStateCreateInfo dynamicState = {};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = 1;
    dynamicState.pDynamicStates = dynamicStates;

    VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
    pipelineCreateInfo.pDynamicState = &dynamicState;
    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.stageCount = shaderStages.size();
    pipelineCreateInfo.pStages = shaderStages.data();
    pipelineCreateInfo.pVertexInputState = &vertexInputCreateInfo;
    pipelineCreateInfo.pInputAssemblyState = &inputAssembly;
    pipelineCreateInfo.pViewportState = &viewportState;
    pipelineCreateInfo.pRasterizationState = &rasterizer;
    pipelineCreateInfo.pColorBlendState = &colorBlending;
    pipelineCreateInfo.pDepthStencilState = &depthStencil;
    pipelineCreateInfo.pMultisampleState = &multisampling;
    pipelineCreateInfo.layout = m_pipelineLayout;
    pipelineCreateInfo.renderPass = core::apiCore.renderPass.renderPass;
    pipelineCreateInfo.subpass = 0;
    pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineCreateInfo.basePipelineIndex = -1;

    if (vkCreateGraphicsPipelines(core::apiCore.device, VK_NULL_HANDLE, 1, &pipelineCreateInfo,
    nullptr, reinterpret_cast<VkPipeline*>(&m_pipeline)) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline");
    }
}

void Drawable::draw(int i) const {
    VkDeviceSize offsets[] = { 0 };

    VkBuffer vertexBuffer = m_shapeInfo.data.vertex.buffer.getBuffer();
    VkBuffer indexBuffer = m_shapeInfo.data.index.buffer.getBuffer();
    vkCmdBindPipeline(core::apiCore.commandBuffers.active[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
    vkCmdBindVertexBuffers(core::apiCore.commandBuffers.active[i], 0, 1, &vertexBuffer, offsets);
    vkCmdBindIndexBuffer(core::apiCore.commandBuffers.active[i], indexBuffer, 0, m_shapeInfo.data.index.type);
    vkCmdBindDescriptorSets(core::apiCore.commandBuffers.active[i], VK_PIPELINE_BIND_POINT_GRAPHICS, 
    m_pipelineLayout, 0, m_descriptorSets.size(), m_descriptorSets.data(), 0, nullptr);
    vkCmdDrawIndexed(core::apiCore.commandBuffers.active[i], m_shapeInfo.data.index.count, m_instanceCount, 0, 0, 0);
}

void destroyPipeline(VkPipeline pipeline) {
    vkDestroyPipeline(core::apiCore.device, pipeline, nullptr);
}

} // namespace age
