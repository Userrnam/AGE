#pragma once

#include <vector>
#include <vulkan/vulkan.h>

#include "Shader.hpp"
#include "Descriptor.hpp"
#include "Core/RenderPassRef.hpp"

namespace age {

struct ObjectCreateInfo {
    bool depthTest;
    float minSampleShading = 0.0f;
    std::vector<Shader> shaders;
    Descriptor descriptor;
    struct {
        VkVertexInputBindingDescription bindingDescription;
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
        VkBuffer buffer;
    } vertex;
    struct {
        uint32_t count;
        VkIndexType type;
        VkBuffer buffer;
    } index;
};

class Object {
    core::RenderPassRef* m_renderPass;
    VkPipelineLayout m_pipelineLayout;
    VkPipeline m_pipeline;
    std::vector<VkDescriptorSet> m_descriptorSets;
    struct {
        VkBuffer buffer;
    } m_vertex;
    struct {
        uint32_t count;
        VkIndexType type;
        VkBuffer buffer;
    } m_index;
protected:
    void create(const ObjectCreateInfo& createInfo);
public:
    void draw(int i);
};

} // namespace age
