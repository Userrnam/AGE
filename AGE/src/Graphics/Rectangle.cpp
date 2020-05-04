#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Rectangle.hpp"
#include "DescriptorManager.hpp"
#include "Core/Core.hpp"
#include "Shader.hpp"

#ifndef CMAKE_DEFINITION
#define SHADER_PATH ""
#endif

namespace age {

struct Vertex {
    glm::vec2 pos;

    static VkVertexInputBindingDescription getBindingDescription() {
		VkVertexInputBindingDescription bindingDescription = {};

		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

    static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() {
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions = {};
		attributeDescriptions.resize(1);

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		return attributeDescriptions;
	}
};

static std::vector<Vertex> verticies = {
    { { -1.0,  1.0 } },
    { {  1.0,  1.0 } },
    { {  1.0, -1.0 } },
    { { -1.0, -1.0 } },
};

static std::vector<uint16_t> indicies = { 0, 1, 2, 2, 3, 0 };

std::vector<Shader> createShaders() {
    std::vector<Shader> out;
    out.resize(2);

    // vertex shader
    out[0].create(SHADER_PATH "rectangle.vert.spv");
    out[0].entry = "main";
    out[0].stage = VK_SHADER_STAGE_VERTEX_BIT;

    // fragment shader
    out[1].create(SHADER_PATH "rectangle.frag.spv");
    out[1].entry = "main";
    out[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;

    return out;
};

core::Descriptor getDescriptor(bool cameraAccess, VkBuffer buffer) {
    std::vector<core::Sampler> samplers;

    core::DescriptorInfo descriptorInfo;
    descriptorInfo.ubos.resize(1);
    descriptorInfo.ubos[0].buffer = buffer;
    descriptorInfo.ubos[0].size = sizeof(RectangleUniform);
    descriptorInfo.ubosBinding = 0;

    auto descriptor = core::getDescriptor(descriptorInfo);

    if (cameraAccess) {
        descriptor.sets.insert(descriptor.sets.begin(), core::apiCore.camera.descriptor);
        descriptor.layouts.insert(descriptor.layouts.begin(), core::apiCore.descriptor.layouts[0].layout);
    }

    return descriptor;
}

void Rectangle::create(bool cameraAccess) {
    core::BufferCreateInfo bufferInfo = {};
    bufferInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    bufferInfo.size = sizeof(RectangleUniform);
    bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

    m_uboBuffer.create(bufferInfo);

    ObjectCreateInfo createInfo;
    createInfo.depthTest = false;
    createInfo.descriptor = getDescriptor(cameraAccess, m_uboBuffer.getBuffer());
    createInfo.index.buffer = core::createDeviceLocalBuffer(indicies.data(), indicies.size() * sizeof(indicies[0]), VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
    createInfo.index.count = indicies.size();
    createInfo.index.type = VK_INDEX_TYPE_UINT16;
    createInfo.minSampleShading = 0.0f;
    createInfo.shaders = createShaders();
    createInfo.vertex.attributeDescriptions = Vertex::getAttributeDescriptions();
    createInfo.vertex.bindingDescription = Vertex::getBindingDescription();
    createInfo.vertex.buffer = core::createDeviceLocalBuffer(verticies.data(), verticies.size() * sizeof(Vertex), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

    createObject(createInfo);

    for (auto& shader : createInfo.shaders) {
        shader.destroy();
    }
}

void Rectangle::destroy() {
    m_uboBuffer.destroy();
    m_vertex.buffer.destroy();
    m_index.buffer.destroy();
    vkDestroyPipeline(core::apiCore.device, m_pipeline, nullptr);
}

void Rectangle::setUniform(const RectangleUniform& uniform) {
    m_uboBuffer.loadData(&uniform, sizeof(uniform));
}

void Rectangle::setColor(const glm::vec4& color) {
    m_color = color;
}

void Rectangle::setSize(const glm::vec2& size) {
    m_size = size;
}

void Rectangle::setPosition(const glm::vec2& pos) {
    m_pos = pos;
}

// void Rectangle::setRotation(const float rotation) {
//     m_rotation = rotation;
// }

void Rectangle::setOrigin(const glm::vec2& origin) {
    m_origin = origin;
}

// FIXME
void Rectangle::upload() {
    RectangleUniform uniform;
    uniform.color = m_color;

    glm::vec2 extent = {
        static_cast<float>(core::apiCore.swapchain.extent.width), 
        static_cast<float>(core::apiCore.swapchain.extent.height),
    };
    glm::vec2 extentInversed = {
        1.0 / extent.x,
        1.0 / extent.y,
    };

    glm::vec2 scale;
    scale.x = m_size.x * extentInversed.x;
    scale.y = m_size.y * extentInversed.y;

    // glm::vec2 origin;
    // origin.x = -(2 * m_origin.x * extentInversed.x - 1);
    // origin.y = -(2 * m_origin.y * extentInversed.y - 1);

    glm::vec2 pos;
    pos.x = 2 * (m_pos.x + m_origin.x) * extentInversed.x - 1;
    pos.y = 2 * (m_pos.y + m_origin.y) * extentInversed.y - 1;

    // uniform.transform = glm::scale(glm::mat4(1.0f), {scale.x, scale.y, 0.0});

    // uniform.transform = glm::rotate(glm::scale(glm::mat4(1.0f), glm::vec3(scale.x, scale.y, 1.0)), m_rotation, glm::vec3(0, 0, 1));

    uniform.transform = {
        scale.x * cos(m_rotation), scale.y * sin(m_rotation), 0, 0,
        -scale.x * sin(m_rotation), scale.y * cos(m_rotation), 0, 0,
        pos.x, pos.y, 1, 0,
        0, 0, 0, 1,
    };

    m_uboBuffer.loadData(&uniform, sizeof(uniform));
}

} // namespace age
