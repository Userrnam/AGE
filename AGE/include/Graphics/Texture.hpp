#pragma once

#include <string>
#include <vulkan/vulkan.h>

#include "Core/Image.hpp"
#include "Sampler.hpp"
#include "Containers/Shared.hpp"

namespace age {

class Texture {
    core::Image m_image;
    Shared<Sampler> m_sampler;

public:
    void destroy();
    VkSampler getSampler() const { return m_sampler.data.getSampler(); }
    const core::Image& getImage() const { return m_image; }

    void create(const std::string& filename, Shared<Sampler> sampler);
    void create(const core::Image& image, Shared<Sampler> sampler);
};

} // namespace age
