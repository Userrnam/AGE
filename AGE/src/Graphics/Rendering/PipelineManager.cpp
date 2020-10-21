#include <unordered_map>

#include "Core/CoreConfig.hpp"
#include "Core/Core.hpp"
#include "PipelineManager.hpp"
#include "RenderPass.hpp"

namespace age {

namespace core {
    extern CoreConfig coreConfig;
}

std::unordered_map<PipelineInfo, std::pair<VkPipeline, VkPipelineLayout>> pipelinesMap;

bool isSame(const std::vector<VkDescriptorSetLayout>& layouts1, const std::vector<VkDescriptorSetLayout>& layouts2) {
    if (layouts1.size() != layouts2.size()) {
        return false;
    }

    for (auto& layout1 : layouts1) {
        bool found = false;
        for (auto& layout2 : layouts2) {
            if (layout2 == layout1) {
                found = true;
                break;
            }
        }
        if (!found) {
            return false;
        }
    }

    return true;
}

// TODO: There is always 2 descriptorSetLayouts. First for camera, second - user defined
// maybe this can be simplified
VkPipelineLayout requestPipelineLayout(const std::vector<VkDescriptorSetLayout>& layouts) {
    for (auto& pipelineLayout : core::apiCore.pipelineLayouts) {
        if (isSame(pipelineLayout.descriptorSetLayouts, layouts)) {
            return pipelineLayout.pipelineLayout;
        }
    }

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = layouts.size();
    pipelineLayoutInfo.pSetLayouts = layouts.data();
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    VkPipelineLayout pipelineLayout;
    if (vkCreatePipelineLayout(core::apiCore.device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
        std::runtime_error("failed to create pipeline layout");
    }

    core::PipelineLayoutRef ref;
    ref.descriptorSetLayouts = layouts;
    ref.pipelineLayout = pipelineLayout;
    core::apiCore.pipelineLayouts.push_back(ref);

    return pipelineLayout;
}

std::pair<VkPipeline, VkPipelineLayout> requestPipeline(PipelineInfo requirements) {
    auto p = pipelinesMap.find(requirements);
    if (p == pipelinesMap.end()) {
        return { 0, 0 };
    }
    return p->second;
}

void destroyPipelineManager() {
    for (auto& p : pipelinesMap) {
        vkDestroyPipeline(core::apiCore.device, p.second.first, nullptr);
    }

	for (auto pipelineLayoutRef : core::apiCore.pipelineLayouts) {
		vkDestroyPipelineLayout(core::apiCore.device, pipelineLayoutRef.pipelineLayout, nullptr);
	}
}

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

void getShaderStageCreateInfos(const std::vector<Shader>& shaders, std::vector<VkPipelineShaderStageCreateInfo>& shaderStages, std::vector<VkSpecializationInfo>& specializationInfos) {
    shaderStages.resize(shaders.size());
    uint32_t specIndex = 0;
    specializationInfos.resize(shaders.size());
    for (size_t i = 0; i < shaders.size(); ++i) {
        shaderStages[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[i].module = shaders[i].getModule();
        shaderStages[i].pName = shaders[i].getEntry().c_str();
        shaderStages[i].stage = shaders[i].getStage();

        if (shaders[i].getSpecialization().getData().size() != 0) {
            specializationInfos[specIndex].dataSize = shaders[i].getSpecialization().getData().size() * sizeof(uint8_t);
            specializationInfos[specIndex].mapEntryCount = shaders[i].getSpecialization().getEntries().size();
            specializationInfos[specIndex].pData = shaders[i].getSpecialization().getData().data();
            specializationInfos[specIndex].pMapEntries = shaders[i].getSpecialization().getEntries().data();

            shaderStages[i].pSpecializationInfo = &specializationInfos[specIndex];

            specIndex++;
        }
    }
}

// Pipeline depends on PipelineLayout, shaders and vertex
// FIXME: currently vertex is ignored
std::pair<VkPipeline, VkPipelineLayout> createPipeline(const PipelineCreateInfo& info) {
    std::pair<VkPipeline, VkPipelineLayout> result;
    result.second = requestPipelineLayout(info.getDescriptorSetLayouts());

    // shaders
    std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
    std::vector<VkSpecializationInfo> specializationInfos;
    getShaderStageCreateInfos(info.getShaders(), shaderStages, specializationInfos);

    // auto& vertexDescription = Shape::getPipelineCreateDescription(info.getShapeId());
    VkVertexInputBindingDescription bindingDescription = {};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(Vector2f);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    VkVertexInputAttributeDescription attributeDescription = {};
    attributeDescription.binding = 0;
    attributeDescription.location = 0;
    attributeDescription.format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescription.offset = 0;

    VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo = {};
    vertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputCreateInfo.vertexBindingDescriptionCount = 1;
    vertexInputCreateInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputCreateInfo.vertexAttributeDescriptionCount = 1;
    vertexInputCreateInfo.pVertexAttributeDescriptions = &attributeDescription;

    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    auto rasterizer = getRasterizationStateCreateInfo();
    auto multisampling = getMultisampleStateCreateInfo();
    auto colorBlendAttachment = getColorBlendAttachment(info.getInfo() & 1);

    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;

    VkDynamicState dynamicStates[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
    VkPipelineDynamicStateCreateInfo dynamicState = {};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = 2;
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
    pipelineCreateInfo.layout = result.second;
    pipelineCreateInfo.renderPass = RenderPass::get();
    pipelineCreateInfo.subpass = 0;
    pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineCreateInfo.basePipelineIndex = -1;

    if (vkCreateGraphicsPipelines(core::apiCore.device, VK_NULL_HANDLE, 1, &pipelineCreateInfo,
    nullptr, &result.first) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline");
    }

    pipelinesMap[info.getInfo()] = result;

    return result;
}

} // namespace age
