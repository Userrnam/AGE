#pragma once

#include <vector>
#include <vulkan/vulkan.h>

#include "Viewport.hpp"
#include "Shader.hpp"
#include "Descriptor.hpp"
#include "Core/RenderPassRef.hpp"
#include "Core/Buffer.hpp"

namespace age {

/*

// note:
Descriptors can be changed at runtime

Shader shader;
Shaders shaders;
shader.createFragmentShader(path, entry=main);
shader.createVertexShader(path, entry=main);
void Drawable.addShader(Shader& shader);

DescriptorInfo info;
info.textures.push_back(texture); // binding 1
info.ubos.push_back(buffer); // binding 0
Descriptor.create(info);

Vertex<T>::fillBinding(bindingDescription);
Vertex<T>::fillAttributes(attributeDescription);

*/

struct ObjectCreateInfo {
    Viewport viewport;
    bool depthTest = false;
    bool multisampling = false;
    bool colorBlending = false;
    float minSampleShading = 0.0f;
    std::vector<Shader> shaders;

    std::vector<Descriptor> descriptors;

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
    uint32_t instanceCount = 1;
};

class Object {
protected:
    core::RenderPassRef* m_renderPass;
    VkPipelineLayout m_pipelineLayout;
    VkPipeline m_pipeline;
    std::vector<void*> m_setPools;
    std::vector<VkDescriptorSet> m_descriptorSets;

    struct {
        core::Buffer buffer;
    } m_vertex;

    struct {
        uint32_t count;
        VkIndexType type;
        core::Buffer buffer;
    } m_index;

    uint32_t m_instanceCount;

    void createObject(const ObjectCreateInfo& createInfo);
public:
    void draw(int i);
};

} // namespace age
