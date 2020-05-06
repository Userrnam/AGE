#pragma once

#include <vector>
#include <vulkan/vulkan.h>

#include "Viewport.hpp"
#include "Shader.hpp"
#include "Core/Descriptor.hpp"
#include "Core/RenderPassRef.hpp"
#include "Core/Buffer.hpp"

namespace age {

struct ObjectCreateInfo {
    Viewport viewport;
    bool depthTest = false;
    bool multisampling = false;
    bool colorBlending = false;
    float minSampleShading = 0.0f;
    std::vector<Shader> shaders;
    core::Descriptor descriptor;
    struct {
        VkVertexInputBindingDescription bindingDescription;
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
        core::Buffer buffer;
    } vertex;
    struct {
        uint32_t count;
        VkIndexType type;
        core::Buffer buffer;
    } index;
};

class Object {
protected:
    core::RenderPassRef* m_renderPass;
    VkPipelineLayout m_pipelineLayout;
    VkPipeline m_pipeline;
    std::vector<VkDescriptorSet> m_descriptorSets;
    struct {
        core::Buffer buffer;
    } m_vertex;
    struct {
        uint32_t count;
        VkIndexType type;
        core::Buffer buffer;
    } m_index;
    void createObject(const ObjectCreateInfo& createInfo);
public:
    void draw(int i);
};

} // namespace age
