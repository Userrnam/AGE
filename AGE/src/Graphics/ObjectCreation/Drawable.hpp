#pragma once

#include <vector>
#include <vulkan/vulkan.h>

#include "View/View.hpp"
#include "Shader.hpp"
#include "Descriptor.hpp"
#include "MemoryHolders/Buffer.hpp"
#include "Vertex.hpp"
#include "Index.hpp"
#include "Containers/Shared.hpp"
#include "ShaderBuilder.hpp"
#include "ShapeManager.hpp"

#include "Components/ComponentCollector.hpp"

namespace age {

class DrawableCreateInfo {
    bool m_colorBlending = false;
    uint32_t m_instanceCount = 1;

    std::vector<Shader> m_shaders;
    std::vector<DescriptorSet> m_descriptors;

    ShapeId m_shapeId;

    friend class Drawable;
public:
    inline DrawableCreateInfo& setShapeId(ShapeId shapeId) {
        m_shapeId = shapeId;
        return *this;
    }

    inline DrawableCreateInfo& setColorBlendEnable(bool flag) {
        m_colorBlending = flag;
        return *this;
    }

    inline DrawableCreateInfo& setIstanceCount(uint32_t count) {
        m_instanceCount = count;
        return *this;
    }

    inline DrawableCreateInfo& addShader(const Shader& shader) {
        m_shaders.push_back(shader);
        return *this;
    }

    inline DrawableCreateInfo& addDescriptorSet(const DescriptorSet& descriptor) {
        m_descriptors.push_back(descriptor);
        return *this;
    }
};

class Drawable {
    void getPipelineLayout(const DrawableCreateInfo& createInfo);
    void getShaderStageCreateInfos(const std::vector<Shader>&, std::vector<VkPipelineShaderStageCreateInfo>&, std::vector<VkSpecializationInfo>&);
protected:
    VkPipelineLayout m_pipelineLayout;
    VkPipeline m_pipeline;
    VkViewport m_viewport;

    std::vector<uint32_t> m_poolIndicies;
    std::vector<VkDescriptorSet> m_descriptorSets;

    ShapeRenderInfo m_shapeRenderInfo;

    uint32_t m_instanceCount = 1;

    void __create(ShapeId, const std::vector<ShaderComponentInfo>&);
public:
    template<typename... Args>
    void create(ShapeId shapeId, Args... components) {
        std::vector<ShaderComponentInfo> _components;
        collectComponents(_components, components...);
        __create(shapeId, _components);
    }

    void create(const DrawableCreateInfo& createInfo);
    void destroy();

    void draw(int i) const;
};

void destroyPipeline(VkPipeline pipeline);

} // namespace age
