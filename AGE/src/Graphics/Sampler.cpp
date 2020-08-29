#include "Sampler.hpp"

#include "Core/Core.hpp"

namespace age {

Sampler defaultSampler;

Sampler& Sampler::create(const SamplerInfo& info) {
    VkSamplerCreateInfo samplerInfo = {};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = info.addressModeU;
    samplerInfo.addressModeW = info.addressModeW;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_TRANSPARENT_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 1.0;
    samplerInfo.mipLodBias = 0.0f;

    if (vkCreateSampler(core::apiCore.device, &samplerInfo, nullptr, &m_sampler) != VK_SUCCESS) {
        throw std::runtime_error("failed to create sampler");
    }
    return *this;
}

void Sampler::destroy() {
    vkDestroySampler(core::apiCore.device, m_sampler, nullptr);
}

void Sampler::createDefault() {
    defaultSampler.create(SamplerInfo());
}

void Sampler::destroyDefault() {
    defaultSampler.destroy();
}

const Sampler& Sampler::getDefault() {
    return defaultSampler;
}

} // namespace age
