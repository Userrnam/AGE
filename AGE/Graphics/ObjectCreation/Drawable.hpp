#pragma once

#include <vector>
#include <vulkan/vulkan.h>

#include "../View/View.hpp"
#include "Shader.hpp"
#include "Descriptor.hpp"
#include "../MemoryHolders/Buffer.hpp"
#include "Index.hpp"
#include "ShapeManager.hpp"
#include "../../Math/Vector.hpp"

#include "Components/ComponentCollector.hpp"

namespace age {

class DrawableCreateInfo {
    bool m_colorBlending = false;
    uint32_t m_instanceCount = 1;

    std::vector<Shader> m_shaders;
    DescriptorSet m_descriptor;

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

    inline DrawableCreateInfo& setDescriptorSet(const DescriptorSet& descriptor) {
        m_descriptor = descriptor;
        return *this;
    }
};

class Drawable {
    void getPipelineLayout(const DrawableCreateInfo& createInfo);
    void getShaderStageCreateInfos(const std::vector<Shader>&, std::vector<VkPipelineShaderStageCreateInfo>&, std::vector<VkSpecializationInfo>&);
protected:
    VkPipelineLayout m_pipelineLayout;
    VkPipeline m_pipeline;

    DescriptorSet m_descriptorSet;

    ShapeRenderInfo m_shapeRenderInfo;

    uint32_t m_instanceCount = 1;

    // position in z coord. Used to determine what will be rendered first
    float m_zPos = 0;

    void __create(ShapeId, const std::vector<ShaderComponentInfo>&);
public:
    float getZ() const { return m_zPos; }

    template<typename... Args>
    void create(ShapeId shapeId, const Args&... components) {
        std::vector<ShaderComponentInfo> _components;
        collectComponents(_components, components...);
        __create(shapeId, _components);
    }

    void addInstanceCount(int i) { assert((int)m_instanceCount + i >= 0); m_instanceCount += i; }
    void setInstanceCount(uint32_t i) { m_instanceCount = i; }
    uint32_t getInstanceCount() const { return m_instanceCount; }

    void create(const DrawableCreateInfo& createInfo);
    void destroy();

    void draw(int i) const;
};

void destroyPipeline(VkPipeline pipeline);

void setZ(float z);
float getZ();

} // namespace age
