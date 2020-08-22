#include "Core.hpp"

#include "PipelineManager.hpp"

namespace age::core {

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

VkPipelineLayout requestPipelineLayout(const std::vector<VkDescriptorSetLayout>& layouts) {
    for (auto& pipelineLayout : apiCore.pipelineLayouts) {
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
    if (vkCreatePipelineLayout(apiCore.device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
        std::runtime_error("failed to create pipeline layout");
    }

    PipelineLayoutRef ref;
    ref.descriptorSetLayouts = layouts;
    ref.pipelineLayout = pipelineLayout;
    apiCore.pipelineLayouts.push_back(ref);

    return pipelineLayout;
}
    
} // namespace age::core
