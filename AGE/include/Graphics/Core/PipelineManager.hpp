#pragma once

#include <vulkan/vulkan.h>
#include <utility>

#include "../Components/ShaderComponent.hpp"

namespace age::core {

struct PipelineInfo {
    bool blendEnable;
    // shaders
};

std::pair<VkPipeline, VkPipelineLayout> requestPipeline(PipelineInfo requirements);

VkPipelineLayout requestPipelineLayout(const std::vector<VkDescriptorSetLayout>& layouts);

} // namespace age::core
