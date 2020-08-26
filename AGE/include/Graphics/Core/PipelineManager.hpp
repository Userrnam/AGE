#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <utility>

#include "../Shader.hpp"
#include "../ShapeManager.hpp"

/*

Pipeline manager can only be used to create pipelines using GraphicsComponents

*/

namespace age::core {

typedef uint64_t PipelineInfo; // first bit is blend enable

class PipelineCreateInfo {
    PipelineInfo m_info;
    ShapeId m_shapeId;
    std::vector<Shader> m_shaders;
    std::vector<VkDescriptorSetLayout> m_layouts;
public:
    inline PipelineInfo getInfo() const { return m_info; }
    inline ShapeId getShapeId() const { return m_shapeId; }
    inline const std::vector<Shader>& getShaders() const { return m_shaders; }
    inline const std::vector<VkDescriptorSetLayout>& getDescriptorSetLayouts() const { return m_layouts; }

    inline PipelineCreateInfo& setInfo(PipelineInfo info) { m_info = info; return *this; }
    inline PipelineCreateInfo& setShapeId(ShapeId shapeId) { m_shapeId = shapeId; return *this; }
    inline PipelineCreateInfo& setShaders(const std::vector<Shader>& shaders) { m_shaders = shaders; return *this; }
    inline PipelineCreateInfo& setLayouts(const std::vector<VkDescriptorSetLayout>& layouts) { m_layouts = layouts; return *this; }

    inline PipelineCreateInfo& addShader(const Shader& shader) { m_shaders.push_back(shader); return *this; }
    inline PipelineCreateInfo& addLayout(VkDescriptorSetLayout layout) { m_layouts.push_back(layout); return *this; }
};

// if VkPipeline is 0 => no such pipeline
std::pair<VkPipeline, VkPipelineLayout> requestPipeline(PipelineInfo requirements);
std::pair<VkPipeline, VkPipelineLayout> createPipeline(const PipelineCreateInfo& info);

VkPipelineLayout requestPipelineLayout(const std::vector<VkDescriptorSetLayout>& layouts);

void destroyPipelineManager();

} // namespace age::core
