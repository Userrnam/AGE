#pragma once

#include <vector>
#include <vulkan/vulkan.h>

#include "View.hpp"
#include "Shader.hpp"
#include "Descriptor.hpp"
#include "Core/RenderPassRef.hpp"
#include "Buffer.hpp"
#include "Vertex.hpp"
#include "Index.hpp"
#include "Containers/Shared.hpp"
#include "ShaderBuilder.hpp"
#include "ShapeManager.hpp"

#include "ComponentCollector.hpp"

namespace age {

class DrawableCreateInfo {
    View* m_view;
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

    inline DrawableCreateInfo& setView(View& view) {
        m_view = &view;
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

    std::vector<uint32_t> m_poolIndicies;
    std::vector<VkDescriptorSet> m_descriptorSets;

    ShapeRenderInfo m_shapeRenderInfo;

    uint32_t m_instanceCount = 1;

    void __create(const View& view, ShapeId, const std::vector<ShaderComponentInfo>&);
public:
    template<typename... Args>
    void create(const View& view, ShapeId shapeId, Args... components) {
        std::vector<ShaderComponentInfo> _components;
        collectComponents(_components, components...);
        __create(view, shapeId, _components);
    }

    void create(const DrawableCreateInfo& createInfo);
    void destroy();

    void draw(int i) const;
};

void destroyPipeline(VkPipeline pipeline);

} // namespace age
