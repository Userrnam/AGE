#pragma once

#include <vulkan/vulkan.h>

namespace age {

struct SamplerInfo {
    VkSamplerAddressMode addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    VkSamplerAddressMode addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

    SamplerInfo& setAddressModeU(VkSamplerAddressMode mode) {
        addressModeU = mode;
        return *this;
    }

    SamplerInfo& setAddressModeW(VkSamplerAddressMode mode) {
        addressModeW = mode;
        return *this;
    }
};

class Sampler {
    VkSampler m_sampler = nullptr;
public:
    inline VkSampler getSampler() const {
        return m_sampler;
    }

    Sampler& create(const SamplerInfo& info);
    void destroy();
};

} // namespace age
