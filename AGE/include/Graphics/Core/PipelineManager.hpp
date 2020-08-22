#pragma once

#include <vulkan/vulkan.h>
#include <utility>

namespace age {

typedef uint64_t ShaderComponentId;

namespace core {

    typedef uint64_t PipelineInfo;

    inline ShaderComponentId __nextId() {
        static ShaderComponentId id = 1;
        id <<= 1; // first bit reserved for color blend (1 enable, 0 disable)
        ShaderComponentId result = id;
        return result;
    }

    std::pair<VkPipeline, VkPipelineLayout> requestPipeline(PipelineInfo requirements);

    VkPipelineLayout requestPipelineLayout(const std::vector<VkDescriptorSetLayout>& layouts);

} // namespace core

template <typename T>
ShaderComponentId getShaderComponentId() {
	static ShaderComponentId id = core::__nextId();
	return id;
}

} // namespace age
