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
#include "ShapeInfo.hpp"
#include "Containers/Shared.hpp"

namespace age {

class DrawableCreateInfo {
    View* m_view;
    bool m_colorBlending = false;
    uint32_t m_instanceCount = 1;

    std::vector<Shader> m_shaders;
    std::vector<DescriptorSet> m_descriptors;

    Shared<ShapeInfo> *m_pShapeInfo;

    friend class Drawable;
public:
    inline DrawableCreateInfo& setShapeInfo(Shared<ShapeInfo>& shape) {
        m_pShapeInfo = &shape;
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
protected:
    VkPipelineLayout m_pipelineLayout;
    VkPipeline m_pipeline;
    std::vector<uint32_t> m_poolIndicies; // used to free descriptor set
    std::vector<VkDescriptorSet> m_descriptorSets;

    Shared<ShapeInfo> m_shapeInfo;

    uint32_t m_instanceCount;

    void createDrawable(const DrawableCreateInfo& createInfo);
public:
    void draw(int i);
};

void destroyPipeline(VkPipeline pipeline);

} // namespace age
