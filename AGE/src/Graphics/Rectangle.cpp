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

static std::vector<Vertex> verticies = {
    { { 0.0, 0.0 } },
    { { 1.0, 0.0 } },
    { { 1.0, 1.0 } },
    { { 0.0, 1.0 } },
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

std::vector<Shader> createTShaders() {
    std::vector<Shader> out;
    out.resize(2);

    // vertex shader
    out[0].create(SHADER_PATH "rectangleT.vert.spv");
    out[0].entry = "main";
    out[0].stage = VK_SHADER_STAGE_VERTEX_BIT;

    // fragment shader
    out[1].create(SHADER_PATH "rectangleT.frag.spv");
    out[1].entry = "main";
    out[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;

    return out;
};

core::Descriptor getDescriptor(VkBuffer buffer) {
    core::DescriptorInfo descriptorInfo;
    descriptorInfo.ubos.resize(1);
    descriptorInfo.ubos[0].buffer = buffer;
    descriptorInfo.ubos[0].size = sizeof(RectangleUniform);
    descriptorInfo.ubosBinding = 0;

    return core::getDescriptor(descriptorInfo);;
}

core::Descriptor getTDescriptor(VkBuffer buffer, const Texture& texture) {
    core::DescriptorInfo descriptorInfo;
    descriptorInfo.ubos.resize(1);
    descriptorInfo.ubos[0].buffer = buffer;
    descriptorInfo.ubos[0].size = sizeof(RectangleUniform);
    descriptorInfo.ubosBinding = 0;

    descriptorInfo.samplers.resize(1);
    descriptorInfo.samplers[0].sampler = texture.getSampler();
    descriptorInfo.samplers[0].view = texture.getImage().getView();
    descriptorInfo.samplersBinding = 1;

    return core::getDescriptor(descriptorInfo);;
}

void Rectangle::preCreate(const View& view, ObjectCreateInfo& createInfo) {
    m_isOwner = true;

    core::BufferCreateInfo bufferInfo = {};
    bufferInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    bufferInfo.size = sizeof(RectangleUniform);
    bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    m_uboBuffer.create(bufferInfo);

    createInfo.depthTest = false;
    createInfo.descriptor.sets.push_back(view.camera.m_descriptor.set);
    createInfo.descriptor.layouts.push_back(view.camera.m_descriptor.layout);

    createInfo.index.buffer = core::createDeviceLocalBuffer(indicies.data(), indicies.size() * sizeof(indicies[0]), VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
    createInfo.index.count = indicies.size();
    createInfo.index.type = VK_INDEX_TYPE_UINT16;
    createInfo.minSampleShading = 0.0f;
    createInfo.vertex.attributeDescriptions = Vertex::getAttributeDescriptions();
    createInfo.vertex.bindingDescription = Vertex::getBindingDescription();
    createInfo.vertex.buffer = core::createDeviceLocalBuffer(verticies.data(), verticies.size() * sizeof(Vertex), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    createInfo.viewport = view.viewport;
}

void Rectangle::create(const Rectangle& sample) {
    m_isOwner = false;

    m_renderPass = sample.m_renderPass;
    m_vertex = sample.m_vertex;
    m_index = sample.m_index;
    m_pipeline = sample.m_pipeline;
    m_pipelineLayout = sample.m_pipelineLayout;
    m_descriptorSets = sample.m_descriptorSets;

    core::BufferCreateInfo bufferInfo = {};
    bufferInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    bufferInfo.size = sizeof(RectangleUniform);
    bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    m_uboBuffer.create(bufferInfo);

    auto descriptor = getDescriptor(m_uboBuffer.getBuffer());
    // replace ubo descriptor
    m_descriptorSets[1] = descriptor.sets[0];
}

void Rectangle::create(const Rectangle& sample, const Texture& texture) {
    m_isOwner = false;

    m_renderPass = sample.m_renderPass;
    m_vertex = sample.m_vertex;
    m_index = sample.m_index;
    m_pipeline = sample.m_pipeline;
    m_pipelineLayout = sample.m_pipelineLayout;
    m_descriptorSets = sample.m_descriptorSets;

    core::BufferCreateInfo bufferInfo = {};
    bufferInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    bufferInfo.size = sizeof(RectangleUniform);
    bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    m_uboBuffer.create(bufferInfo);

    auto descriptor = getTDescriptor(m_uboBuffer.getBuffer(), texture);
    // replace ubo descriptor
    m_descriptorSets[1] = descriptor.sets[0];
}

void Rectangle::create(const View& view, bool colorBlending) {
    ObjectCreateInfo createInfo;
    createInfo.colorBlending = colorBlending;
    preCreate(view, createInfo);

    auto d = getDescriptor(m_uboBuffer.getBuffer());
    createInfo.descriptor.sets.push_back(d.sets[0]);
    createInfo.descriptor.layouts.push_back(d.layouts[0]);
    createInfo.shaders = createShaders();

    createObject(createInfo);

    for (auto& shader : createInfo.shaders) {
        shader.destroy();
    }
}

void Rectangle::create(const View& view, const Texture& texture, bool colorBlending) {
    ObjectCreateInfo createInfo;
    createInfo.colorBlending = colorBlending;
    preCreate(view, createInfo);

    auto d = getTDescriptor(m_uboBuffer.getBuffer(), texture);
    createInfo.descriptor.sets.push_back(d.sets[0]);
    createInfo.descriptor.layouts.push_back(d.layouts[0]);
    createInfo.shaders = createTShaders();

    createObject(createInfo);

    for (auto& shader : createInfo.shaders) {
        shader.destroy();
    }
}

Rectangle::~Rectangle() {
    m_uboBuffer.destroy();
    if (m_isOwner) {
        m_vertex.buffer.destroy();
        m_index.buffer.destroy();
        vkDestroyPipeline(core::apiCore.device, m_pipeline, nullptr);
    }
}

void Rectangle::uploadUniform(const RectangleUniform& uniform) {
    m_uboBuffer.loadData(&uniform, sizeof(uniform));
}

void Rectangle::setColor(const glm::vec4& color) {
    m_color = color;
}

void Rectangle::setColor(float r, float g, float b, float a) {
    m_color.r = r;
    m_color.g = g;
    m_color.b = b;
    m_color.a = a;
}

// FIXME
void Rectangle::upload() {
    RectangleUniform uniform;
    uniform.color = m_color;
    uniform.transform = getTransform();

    m_uboBuffer.loadData(&uniform, sizeof(uniform));
}

} // namespace age
