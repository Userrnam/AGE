#include <vulkan/vulkan.h>

#include "Vertex.hpp"

namespace age::__ {

void fillVertexBindingDescription(uint32_t vertexSize, void* pDescription) {
    VkVertexInputBindingDescription* description = reinterpret_cast<VkVertexInputBindingDescription*>(pDescription);
    description->binding = 0;
    description->stride = vertexSize;
    description->inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
}

void fillAttributeDescriptions(std::vector<VertexAttribute>& attributes, void* vpDescriptions) {
    auto* pDescriptions = reinterpret_cast<std::vector<VkVertexInputAttributeDescription>*>(vpDescriptions);
    pDescriptions->resize(attributes.size());
    VkVertexInputAttributeDescription* descriptions = reinterpret_cast<VkVertexInputAttributeDescription*>(pDescriptions->data());

    size_t descriptionIndex = 0;
    uint32_t offset = 0;
    for (size_t i = 0; i < attributes.size(); ++i) {
        descriptions[descriptionIndex].binding = 0;
        descriptions[descriptionIndex].location = i;
        descriptions[descriptionIndex].offset = offset;
        descriptions[descriptionIndex].format = static_cast<VkFormat>(attributes[i].format);

        descriptionIndex += sizeof(VkVertexInputAttributeDescription);
        offset += getFormatSize(attributes[i].format);
    }
}

} // namespace age::__
