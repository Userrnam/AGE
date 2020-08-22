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
#include "ShaderBuilder.hpp"

namespace age {

class DrawableCreateInfo {
    View* m_view;
    bool m_colorBlending = false;
    uint32_t m_instanceCount = 1;

    std::vector<Shader> m_shaders;
    std::vector<DescriptorSet> m_descriptors;

    Shared<ShapeInfo> *m_pShapeInfo;

    friend class Drawable;

    template<typename Head, typename...Tail>
    void collectComponents(std::vector<ShaderComponentInfo>& components, Head head, Tail... tail) {
        components.push_back(head.getInfo());
        collectComponents(tail...);
    }
    template<typename Head>
    void collectComponents(std::vector<ShaderComponentInfo>& components, Head head) {
        components.push_back(head.getInfo());
    }
public:
    template<typename... Components>
    DrawableCreateInfo& createBasedOnComponents(Components... components) {
        // std::vector<ShaderComponentInfo> _components;
        // collectComponents(_components, components...);

        // // generate shaders
        // ShaderBuilder sb;

        // m_shaders.push_back(sb.compileVertexShader(_components));
        // m_shaders.push_back(sb.compileFragmentShader(_components));

        // // add descriptorSet
        // m_descriptors.push_back(
        //     DescriptorSet()
        //     .get(
        //         DescriptorSetInfo()
        //         .getBasedOnComponents(_components)
        //     )
        // );
    }

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
    void getPipelineLayout(const DrawableCreateInfo& createInfo);
    void getShaderStageCreateInfos(const std::vector<Shader>&, std::vector<VkPipelineShaderStageCreateInfo>&, std::vector<VkSpecializationInfo>&);
protected:
    VkPipelineLayout m_pipelineLayout;
    VkPipeline m_pipeline;
    std::vector<uint32_t> m_poolIndicies;
    std::vector<VkDescriptorSet> m_descriptorSets;

    Shared<ShapeInfo> m_shapeInfo;

    uint32_t m_instanceCount;

public:
    void create(const DrawableCreateInfo& createInfo);
    void destroy();

    void draw(int i) const;
};

void destroyPipeline(VkPipeline pipeline);

} // namespace age
