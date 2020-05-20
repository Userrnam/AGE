#include <vulkan/vulkan.h>

#include "Format.hpp"

#define FORMAT_SIZE(formatPartial, floatCase, size) \
    case VK_ ## formatPartial ## _UNORM: \
    case VK_ ## formatPartial ## _SNORM: \
    case VK_ ## formatPartial ## _USCALED: \
    case VK_ ## formatPartial ## _SSCALED: \
    case VK_ ## formatPartial ## _UINT: \
    case VK_ ## formatPartial ## _SINT: \
    case VK_ ## formatPartial ## floatCase: \
    return size

#define FORMAT_SIZE_BIG(formatPartial, size) \
    case VK_ ## formatPartial ## _UINT: \
    case VK_ ## formatPartial ## _SINT: \
    case VK_ ## formatPartial ## _SFLOAT: \
    return size

namespace age {

uint32_t getFormatSize(VkFormat format) {
    switch (format) {
        FORMAT_SIZE(FORMAT_R8, _SRGB, sizeof(uint8_t));
        FORMAT_SIZE(FORMAT_R8G8, _SRGB, 2 * sizeof(uint8_t));
        FORMAT_SIZE(FORMAT_R8G8B8, _SRGB, 3 * sizeof(uint8_t));
        FORMAT_SIZE(FORMAT_B8G8R8, _SRGB, 3 * sizeof(uint8_t));
        FORMAT_SIZE(FORMAT_R8G8B8A8, _SRGB, 4 * sizeof(uint8_t));
        FORMAT_SIZE(FORMAT_B8G8R8A8, _SRGB, 4 * sizeof(uint8_t));

        FORMAT_SIZE(FORMAT_R16, _SFLOAT, sizeof(uint16_t));
        FORMAT_SIZE(FORMAT_R16G16, _SFLOAT, 2 * sizeof(uint16_t));
        FORMAT_SIZE(FORMAT_R16G16B16, _SFLOAT, 3 * sizeof(uint16_t));
        FORMAT_SIZE(FORMAT_R16G16B16A16, _SFLOAT, 4 * sizeof(uint16_t));

        FORMAT_SIZE_BIG(FORMAT_R32, sizeof(uint32_t));
        FORMAT_SIZE_BIG(FORMAT_R32G32, 2 * sizeof(uint32_t));
        FORMAT_SIZE_BIG(FORMAT_R32G32B32, 3 * sizeof(uint32_t));
        FORMAT_SIZE_BIG(FORMAT_R32G32B32A32, 4 * sizeof(uint32_t));

        FORMAT_SIZE_BIG(FORMAT_R64, sizeof(uint64_t));
        FORMAT_SIZE_BIG(FORMAT_R64G64, 2 * sizeof(uint64_t));
        FORMAT_SIZE_BIG(FORMAT_R64G64B64, 3 * sizeof(uint64_t));
        FORMAT_SIZE_BIG(FORMAT_R64G64B64A64, 4 * sizeof(uint64_t));

    default:
        return 0;
    }
}

} // namespace age
