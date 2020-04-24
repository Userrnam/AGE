#include <vulkan/vulkan.h>
#include <stdexcept>

#include "stb_image.h"

#include "Texture.hpp"


namespace age {

void Texture::load(const char* filename) {
    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load(filename, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

    if (!pixels) {
        std::runtime_error("failed to load image");
    }

    
}

}
