#pragma once

#include <string>
#include <vulkan/vulkan.h>

#include "Core/Image.hpp"
#include "Sampler.hpp"

namespace age {

class Texture {
    core::Image m_image;
    Sampler m_sampler;
public:
    void destroy();
    VkSampler getSampler() const { return m_sampler.getSampler(); }
    const core::Image& getImage() const { return m_image; }

    void create(const std::string& filename, Sampler sampler);
    void create(const core::Image& image, Sampler sampler);
};

} // namespace age
