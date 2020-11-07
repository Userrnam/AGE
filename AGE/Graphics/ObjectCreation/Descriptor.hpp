#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "../../Containers/TaggedUnion.hpp"

#include "../MemoryHolders/Buffer.hpp"
#include "../Objects/Texture.hpp"

#include "Components/ShaderComponent.hpp"
#include "DD.hpp"

namespace age {

class DescriptorBinding {
    VkShaderStageFlags m_stage = 0;
    VkDescriptorType m_descriptorType;
    TaggedUnion<Buffer, Texture> m_descriptor;
public:
    inline VkShaderStageFlags getStage() const { return m_stage; }
    inline VkDescriptorType getDescriptorType() const { return m_descriptorType; }
    inline const TaggedUnion<Buffer, Texture>& getDescriptor() const {
        return m_descriptor;
    }

    inline DescriptorBinding& setDescriptorType(VkDescriptorType type) {
        m_descriptorType = type;
        return *this;
    }

    inline DescriptorBinding& setStage(VkShaderStageFlags stage) {
        m_stage = stage;
        return *this;
    }

    inline DescriptorBinding& setDescriptor(const TaggedUnion<Buffer, Texture>& bt) {
        m_descriptor = bt;
        return *this;
    }

    inline DescriptorBinding& setDescriptor(const Buffer& buffer) {
        m_descriptor = buffer;
        return *this;
    }

    inline DescriptorBinding& add(const Texture& texture) {
        m_descriptor = texture;
        return *this;
    }
};

class DescriptorSetInfo {
    std::vector<DescriptorBinding> m_bindings;
    uint32_t m_setCount = 16;

    friend class DescriptorSet;
    friend VkDescriptorSetLayout createDescriptorSetLayout(const DescriptorSetInfo& info);
    friend VkDescriptorSetLayout requestDescriptorSetLayout(const DescriptorSetInfo& info);

    void __addBasedOnComponent(const ShaderComponentInfo& info) {
        for (auto& elem : info.m_data) {
            if (std::holds_alternative<ShaderComponentBuffer>(elem)) {
                DescriptorBinding binding = {};

                auto b = std::get<ShaderComponentBuffer>(elem);
                binding.setStage(VK_SHADER_STAGE_VERTEX_BIT);
                binding.setDescriptor(b.m_buffer);
                if (b.m_arrayIndex == "") {
                    binding.setDescriptorType(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
                } else {
                    binding.setDescriptorType(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);
                }

                this->addBinding(binding);
            } else if (std::holds_alternative<ShaderComponentTexture>(elem)) {
                this->addBinding(
                    DescriptorBinding()
                    .setStage(VK_SHADER_STAGE_FRAGMENT_BIT)
                    .setDescriptorType(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
                    .add(std::get<ShaderComponentTexture>(elem).m_texture)
                );
            }
        }
    }

public:
    inline uint32_t getBindingCount() const { return m_bindings.size(); }

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
    DescriptorSetInfo getBasedOnComponents(Head head, Tail... tail) {
        __addBasedOnComponent(head.getInfo());
        return getBasedOnComponents(tail...);
    }
    template<typename Head>
    DescriptorSetInfo& getBasedOnComponents(Head head) {
        __addBasedOnComponent(head.getInfo());
        return *this;
    }

    DescriptorSetInfo& getBasedOnComponents(const std::vector<ShaderComponentInfo>& components) {
        for (auto& component : components) {
            __addBasedOnComponent(component);
        }
        return *this;
    }
};

class DescriptorSet {
    DD m_dd;
public:
    VkDescriptorSetLayout getLayout() const { return m_dd.layout; }
    VkDescriptorSet getSet() const { return m_dd.set; }

    DescriptorSet& get(const DescriptorSetInfo& info);

    void destroy();
};

} // namespace age
