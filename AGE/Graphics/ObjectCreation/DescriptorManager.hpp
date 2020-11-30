#pragma once

#include <vector>
#include <unordered_map>
#include <vulkan/vulkan.h>
#include <stdexcept>

#include "DD.hpp"

namespace age {

#define NONE_TID    0b00
#define STORAGE_TID 0b01
#define UNIFORM_TID 0b10
#define TEXTURE_TID 0b11

// 4 types 2bit
// 00 - none
// 01 - StorgeBuffer
// 10 - UniformBuffer
// 11 - Texture
// 64 bit int can have 32 types

// cannot contain more than 32 types
struct DescriptorInfo {
    uint64_t types = 0;

    DescriptorInfo() {}
    DescriptorInfo(uint64_t t) : types(t) {}

    bool operator==(const DescriptorInfo& other) const {
        return other.types == types;
    }

    inline void addType(VkDescriptorType t) {
        uint64_t tid;
        switch ((uint32_t)t) {
        case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
            tid = STORAGE_TID;
            break;
        case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
            tid = UNIFORM_TID;
            break;
        case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
            tid = TEXTURE_TID;
            break;
        default:
            throw std::runtime_error("DescriptorInfo: Unknown DescriptorType");
        }

        // add tid to types
        types <<= 2;
        types |= tid;
    }

    // true - able to fetch
    bool fetch(uint32_t index, VkDescriptorType& type, VkShaderStageFlags& flags) const {
        uint64_t tid = types >> (2 * index);
        tid &= 0b11;

        switch (tid) {
        case NONE_TID:
            return false;
        case STORAGE_TID:
            type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            flags = VK_SHADER_STAGE_VERTEX_BIT;
            return true;
        case UNIFORM_TID:
            type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            flags = VK_SHADER_STAGE_VERTEX_BIT;
            return true;
        case TEXTURE_TID:
            type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            flags = VK_SHADER_STAGE_FRAGMENT_BIT;
            return true;
        default:
            throw std::runtime_error("DescriptorInfo: Unknown DescriptorType");
        }
    }

    uint32_t count() const {
        uint32_t result = 0;
        uint64_t tt = types;

        while (tt & 0b11) {
            result++;
            tt >>= 2;
        }

        return result;
    }
};

struct DescriptorPool {
    VkDescriptorPool pool;
    std::vector<VkDescriptorSet> occupiedSets;
    std::vector<VkDescriptorSet> freeSets;

    bool getSet(VkDescriptorSet& set) {
        if (freeSets.size()) {
            set = freeSets.back();

            freeSets.pop_back();
            occupiedSets.push_back(set);

            return true;
        }

        return false;
    }

    bool freeSet(VkDescriptorSet set) {
        for (uint32_t i = 0; i < occupiedSets.size(); ++i) {
            if (occupiedSets[i] == set) {
                freeSets.push_back(set);
                occupiedSets.erase(occupiedSets.begin() + i);

                return true;
            }
        }

        return false;
    }

    bool isFree() {
        return occupiedSets.size() == 0;
    }

    void status();
};

class DescriptorManager {
    static std::unordered_map<DescriptorInfo, VkDescriptorSetLayout> m_layouts;
    static std::unordered_map<DescriptorInfo, std::vector<DescriptorPool>> m_pools;
    static VkDescriptorSetLayout createLayout(DescriptorInfo info);
    static VkDescriptorSet createSets(DescriptorInfo info, uint32_t setCount, VkDescriptorSetLayout layout);
public:
    static DD requestDescriptorSet(DescriptorInfo info);
    static void freeDescriptorSet(const DD& dd);
    static void destroy();
};

} // namespace age

template<>
struct std::hash<age::DescriptorInfo> {
    std::size_t operator()(age::DescriptorInfo info) const noexcept {
        return std::hash<uint64_t>{}(info.types);
    }
};
