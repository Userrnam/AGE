#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <variant>

#include "Buffer.hpp"
#include "Texture.hpp"

#include "Debug.hpp"

#include "Components/ShaderComponent.hpp"

namespace age {

class DescriptorBinding {
    VkShaderStageFlags m_stage = 0;
    VkDescriptorType m_descriptorType;
    std::vector<std::variant<Buffer, Texture>> m_descriptors;
public:
    inline VkShaderStageFlags getStage() const { return m_stage; }
    inline VkDescriptorType getDescriptorType() const { return m_descriptorType; }
    inline const std::vector<std::variant<Buffer, Texture>>& getDescriptors() const {
        return m_descriptors;
    }

    inline DescriptorBinding& setDescriptorType(VkDescriptorType type) {
        m_descriptorType = type;
        return *this;
    }

    inline DescriptorBinding& setStage(VkShaderStageFlags stage) {
        m_stage = stage;
        return *this;
    }

    inline DescriptorBinding& add(Buffer& buffer) {
        m_descriptors.push_back(buffer);
        return *this;
    }

    inline DescriptorBinding& add(std::variant<Buffer, Texture> bt) {
        m_descriptors.push_back(bt);
        return *this;
    }

    inline DescriptorBinding& add(Texture& texture) {
        m_descriptors.push_back(texture);
        return *this;
    }
};

class DescriptorSetInfo {
    std::vector<DescriptorBinding> m_bindings;
    uint32_t m_setCount = 16;

    friend class DescriptorSet;
    friend VkDescriptorSetLayout createDescriptorSetLayout(const DescriptorSetInfo& info);
    friend VkDescriptorSetLayout requestDescriptorSetLayout(const DescriptorSetInfo& info);
public:
    inline DescriptorSetInfo& addBinding(const DescriptorBinding& binding) {
        m_bindings.push_back(binding);
        return *this;
    }

    // number of descriptor sets that will be allocated
    inline DescriptorSetInfo& setSetCount(uint32_t count) {
        m_setCount = count;
        return *this;
    }

    template<typename Head, typename...Tail>
    DescriptorSetInfo get(Head head, Tail... tail) {
        auto description = head.getInfo().m_description;
        this->addBinding(
            DescriptorBinding()
            .setDescriptorType(description.m_type)
            .setStage(description.m_stage)
            .add(description.m_descriptor)
        );
        return get(tail...);
    }
    template<typename Head>
    DescriptorSetInfo& get(Head head) {
        auto description = head.getInfo().m_description;
        this->addBinding(
            DescriptorBinding()
            .setDescriptorType(description.m_type)
            .setStage(description.m_stage)
            .add(description.m_descriptor)
        );
        return *this;
    }
};

void freeDescriptor(uint32_t poolIndex, VkDescriptorSet descriptor);

class DescriptorSet {
    uint32_t m_poolIndex = 0;
    VkDescriptorSetLayout m_layout = VK_NULL_HANDLE; // used to create pipeline
    VkDescriptorSet m_set = VK_NULL_HANDLE;

    VkDescriptorSet requestDescriptorSet(const DescriptorSetInfo& info);
    VkDescriptorSet createDescriptorSets(const DescriptorSetInfo& info);

    friend class Drawable;
public:


    VkDescriptorSetLayout getLayout() { return m_layout; }
    VkDescriptorSet getSet() { return m_set; }

    DescriptorSet& get(const DescriptorSetInfo& info);
    inline void free() {
        freeDescriptor(m_poolIndex, m_set);
    }
};

} // namespace age
