#include "Drawable.hpp"

#include "Core/Core.hpp"
#include "Core/CoreConfig.hpp"
#include "Rendering/RenderPassManager.hpp"
#include "Rendering/PipelineManager.hpp"
#include "View/ViewManager.hpp"
#include "Core/Pool.hpp"

namespace age::core {
    extern CoreConfig coreConfig;
}

namespace age {

constexpr inline VkPipelineRasterizationStateCreateInfo getRasterizationStateCreateInfo() {
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

    return rasterizer;
}

inline VkPipelineColorBlendAttachmentState getColorBlendAttachment(bool blendEnable) {
    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};

    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                                          VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

    colorBlendAttachment.blendEnable = blendEnable;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;

    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    return colorBlendAttachment;
}

inline VkPipelineMultisampleStateCreateInfo getMultisampleStateCreateInfo() {
    VkPipelineMultisampleStateCreateInfo multisampling = {};

    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    if (core::coreConfig.multisampling.sampleCount != VK_SAMPLE_COUNT_1_BIT) {
        multisampling.rasterizationSamples = core::apiCore.multisampling.sampleCount;
        multisampling.sampleShadingEnable = core::coreConfig.multisampling.sampleRateShading;
        multisampling.minSampleShading = core::coreConfig.multisampling.minSampleShading;
    } else {
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    }

    return multisampling;
}

void Drawable::getPipelineLayout(const DrawableCreateInfo& info) {
    std::vector<VkDescriptorSetLayout> layouts;
    layouts.reserve(info.m_descriptors.size() + 1);
    m_poolIndicies.reserve(info.m_descriptors.size() + 1);
    m_descriptorSets.reserve(info.m_descriptors.size() + 1);

    // add camera descriptor
    auto& view = ViewManager::getSelected();
    m_viewport = view.getViewport();
    auto cameraDescriptor = view.getDescriptor();
    m_descriptorSets.push_back(cameraDescriptor.m_set);
    m_poolIndicies.push_back(cameraDescriptor.m_poolIndex);
    layouts.push_back(cameraDescriptor.m_layout);

    for (auto& d : info.m_descriptors) {
        m_descriptorSets.push_back(d.m_set);
        m_poolIndicies.push_back(d.m_poolIndex);
        layouts.push_back(d.m_layout);
    }

    m_pipelineLayout = requestPipelineLayout(layouts);
}

void Drawable::getShaderStageCreateInfos(const std::vector<Shader>& shaders, std::vector<VkPipelineShaderStageCreateInfo>& shaderStages, std::vector<VkSpecializationInfo>& specializationInfos) {
    shaderStages.resize(shaders.size());
    uint32_t specIndex = 0;
    specializationInfos.resize(shaders.size());
    for (size_t i = 0; i < shaders.size(); ++i) {
        shaderStages[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[i].module = shaders[i].getModule();
        shaderStages[i].pName = shaders[i].getEntry().c_str();
        shaderStages[i].stage = shaders[i].getStage();

        if (shaders[i].getSpecialization().m_data.size() != 0) {
            specializationInfos[specIndex].dataSize = shaders[i].getSpecialization().m_data.size() * sizeof(uint8_t);
            specializationInfos[specIndex].mapEntryCount = shaders[i].getSpecialization().m_entries.size();
            specializationInfos[specIndex].pData = shaders[i].getSpecialization().m_data.data();
            specializationInfos[specIndex].pMapEntries = shaders[i].getSpecialization().m_entries.data();

            shaderStages[i].pSpecializationInfo = &specializationInfos[specIndex];

            specIndex++;
        }
    }
}

void Drawable::create(const DrawableCreateInfo& info) {
    m_shapeRenderInfo = Shape::get(info.m_shapeId);
    m_instanceCount = info.m_instanceCount;

    // get pipeline layouts, store descriptorSets
    getPipelineLayout(info);

    // shaders
    std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
    std::vector<VkSpecializationInfo> specializationInfos;
    getShaderStageCreateInfos(info.m_shaders, shaderStages, specializationInfos);

    auto& vertexDescription = Shape::getPipelineCreateDescription(info.m_shapeId);

    VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo = {};
    vertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputCreateInfo.vertexBindingDescriptionCount = 1;
    vertexInputCreateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexDescription.attributeDescriptions.size());
    vertexInputCreateInfo.pVertexBindingDescriptions = &vertexDescription.bindingDescription;
    vertexInputCreateInfo.pVertexAttributeDescriptions = vertexDescription.attributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkRect2D scissors = {};
    scissors.offset = { 0, 0 };
    scissors.extent = core::apiCore.swapchain.extent;

    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissors;

    auto rasterizer = getRasterizationStateCreateInfo();
    auto multisampling = getMultisampleStateCreateInfo();
    auto colorBlendAttachment = getColorBlendAttachment(info.m_colorBlending);

    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;

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

void Drawable::__create(ShapeId shapeId, const std::vector<ShaderComponentInfo>& compoents) {
    m_shapeRenderInfo = Shape::get(shapeId);

    std::vector<VkDescriptorSetLayout> layouts;
    // add camera descriptor
    auto& view = ViewManager::getSelected();
    m_viewport = view.getViewport();
    auto cameraDescriptor = view.getDescriptor();
    m_descriptorSets.push_back(cameraDescriptor.m_set);
    m_poolIndicies.push_back(cameraDescriptor.m_poolIndex);
    layouts.push_back(cameraDescriptor.m_layout);

    auto descriptor = DescriptorSet().get(
        DescriptorSetInfo().getBasedOnComponents(compoents)
    );

    m_descriptorSets.push_back(descriptor.m_set);
    m_poolIndicies.push_back(descriptor.m_poolIndex);
    layouts.push_back(descriptor.m_layout);

    // color blend is always true
    // TODO: remove color blend option from everywhere
    PipelineInfo pipelineInfo = 1;
    for (auto& component : compoents) {
        pipelineInfo |= component.m_id;
    }
    auto pipeline = requestPipeline(pipelineInfo);
    if (!pipeline.first) {
        ShaderBuilder builder;
        auto vertexShader = builder.compileVertexShader(compoents);
        auto fragmentShader = builder.compileFragmentShader(compoents);
        
        pipeline = createPipeline(
            PipelineCreateInfo()
            .setInfo(pipelineInfo)
            .setShapeId(shapeId)
            .setLayouts(layouts)
            .addShader(vertexShader)
            .addShader(fragmentShader)
        );

        vertexShader.destroy();
        fragmentShader.destroy();
    }
    
    m_pipeline = pipeline.first;
    m_pipelineLayout = pipeline.second;
}

void Drawable::destroy() {
    for (int i = 1; i < m_descriptorSets.size(); ++i) {
        freeDescriptor(m_poolIndicies[i], m_descriptorSets[i]);
    }
}

void Drawable::draw(int i) const {
    VkBuffer vertexBuffer = m_shapeRenderInfo.m_vertexMemoryId.buffer;
    VkBuffer indexBuffer = m_shapeRenderInfo.m_indexMemoryId.buffer;

    vkCmdSetViewport(core::apiCore.commandBuffers.active[i], 0, 1, &m_viewport);
    VkDeviceSize offsets[] = { m_shapeRenderInfo.m_vertexMemoryId.address };

    vkCmdBindPipeline(core::apiCore.commandBuffers.active[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
    vkCmdBindVertexBuffers(core::apiCore.commandBuffers.active[i], 0, 1, &vertexBuffer, offsets);
    vkCmdBindIndexBuffer(core::apiCore.commandBuffers.active[i], indexBuffer, m_shapeRenderInfo.m_indexMemoryId.address, m_shapeRenderInfo.m_indexType);
    vkCmdBindDescriptorSets(core::apiCore.commandBuffers.active[i], VK_PIPELINE_BIND_POINT_GRAPHICS, 
    m_pipelineLayout, 0, m_descriptorSets.size(), m_descriptorSets.data(), 0, nullptr);
    vkCmdDrawIndexed(core::apiCore.commandBuffers.active[i], m_shapeRenderInfo.m_indexCount, m_instanceCount, 0, 0, 0);
}

void destroyPipeline(VkPipeline pipeline) {
    vkDestroyPipeline(core::apiCore.device, pipeline, nullptr);
}

} // namespace age
