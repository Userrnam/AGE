#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

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

// static std::vector<Vertex> verticies = {
//     { { -1.0,  1.0 } },
//     { {  1.0,  1.0 } },
//     { {  1.0, -1.0 } },
//     { { -1.0, -1.0 } },
// };

static std::vector<Vertex> verticies = {
    { { 0.0, 1.0 } },
    { { 1.0, 1.0 } },
    { { 1.0, 0.0 } },
    { { 0.0, 0.0 } },
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

// FIXME
void Rectangle::upload() {
    RectangleUniform uniform;
    uniform.color = m_color;
    uniform.transform = getTransform();

    m_uboBuffer.loadData(&uniform, sizeof(uniform));
}

} // namespace age
