#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Rectangle.hpp"
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

Shaders createCShaders() {
    Shaders out;

    out.addVertexShader(SHADER_PATH "rectangleC.vert.spv");
    out.addFragmentShader(SHADER_PATH "rectangleC.frag.spv");

    return out;
};

Shaders createFactoryShaders(uint32_t count) {
    Shaders out;
    ShaderSpecialization specialization;

    specialization.add<uint32_t>(count);
    out.addVertexShader(SHADER_PATH "factoryRectangleC.vert.spv", "main", specialization);
    out.addFragmentShader(SHADER_PATH "rectangleC.frag.spv");

    return out;
};

Shaders createTexturedFactoryShaders(uint32_t count) {
    Shaders out;

    out.addVertexShader(SHADER_PATH "factoryRectangleT.vert.spv");
    out.addFragmentShader(SHADER_PATH "rectangleT.frag.spv");

    return out;
};

Shaders createCTShaders() {
    Shaders out;

    out.addVertexShader(SHADER_PATH "rectangleCT.vert.spv");
    out.addFragmentShader(SHADER_PATH "rectangleCT.frag.spv");

    return out;
};

Descriptor getDescriptor(core::Buffer& buffer) {
    DescriptorInfo descriptorInfo;
    descriptorInfo.ubos.resize(1);
    descriptorInfo.ubos[0] = &buffer;

    Descriptor descriptor;
    descriptor.get(descriptorInfo);
    return descriptor;
}

Descriptor getTDescriptor(core::Buffer& buffer, uint32_t size, Texture& texture) {
    DescriptorInfo descriptorInfo;
    descriptorInfo.ubos.resize(1);
    descriptorInfo.ubos[0] = &buffer;

    descriptorInfo.textures.resize(1);
    descriptorInfo.textures[0] = &texture;

    Descriptor descriptor;
    descriptor.get(descriptorInfo);

    return descriptor;
}

void Rectangle::preCreate(View& view, ObjectCreateInfo& createInfo) {
    m_isOwner = true;

    core::BufferCreateInfo bufferInfo = {};
    bufferInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    bufferInfo.size = sizeof(RectangleUniform);
    bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    m_uboBuffer.create(bufferInfo);

    createInfo.depthTest = false;

    createInfo.descriptors.push_back(view.getCamera().getDescriptor());

    createInfo.index.buffer = core::createDeviceLocalBuffer(indicies.data(), indicies.size() * sizeof(indicies[0]), VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
    createInfo.index.count = indicies.size();
    createInfo.index.type = VK_INDEX_TYPE_UINT16;
    createInfo.minSampleShading = 0.0f;
    createInfo.vertex.attributeDescriptions = Vertex::getAttributeDescriptions();
    createInfo.vertex.bindingDescription = Vertex::getBindingDescription();
    createInfo.vertex.buffer = core::createDeviceLocalBuffer(verticies.data(), verticies.size() * sizeof(Vertex), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    createInfo.viewport = view.getViewport();
}

void Rectangle::create(const Rectangle& sample) {
    m_isOwner = false;

    m_renderPass = sample.m_renderPass;
    m_vertex = sample.m_vertex;
    m_index = sample.m_index;
    m_pipeline = sample.m_pipeline;
    m_pipelineLayout = sample.m_pipelineLayout;
    m_descriptorSets = sample.m_descriptorSets;
    m_instanceCount = sample.m_instanceCount;

    core::BufferCreateInfo bufferInfo = {};
    bufferInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    bufferInfo.size = sizeof(RectangleUniform);
    bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    m_uboBuffer.create(bufferInfo);

    auto descriptor = getDescriptor(m_uboBuffer);
    // replace ubo descriptor
    m_descriptorSets[1] = descriptor.getSet();
}

void Rectangle::create(const Rectangle& sample, Texture& texture) {
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

    auto descriptor = getTDescriptor(m_uboBuffer, sizeof(RectangleUniform), texture);
    // replace ubo descriptor
    m_descriptorSets[1] = descriptor.getSet();
}

void Rectangle::create(View& view, bool colorBlending) {
    ObjectCreateInfo createInfo;
    createInfo.colorBlending = colorBlending;
    preCreate(view, createInfo);

    createInfo.descriptors.push_back(getDescriptor(m_uboBuffer));
    createInfo.shaders = createCShaders();

    createObject(createInfo);

    createInfo.shaders.destroy();
}

void Rectangle::create(View& view, Texture& texture, bool colorBlending) {
    ObjectCreateInfo createInfo;
    createInfo.colorBlending = colorBlending;
    preCreate(view, createInfo);

    createInfo.descriptors.push_back(getTDescriptor(m_uboBuffer, sizeof(RectangleUniform), texture));
    createInfo.shaders = createCTShaders();

    createObject(createInfo);

    createInfo.shaders.destroy();
}

void Rectangle::destroy() {
    m_uboBuffer.destroy();
    freeDescriptor(m_setPools[1], m_descriptorSets[1]);
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

void RectangleFactory::addChild(RectangleInstance& instance) {
    if (m_instanceCount >= m_count) {
        throw std::runtime_error("RectangleFactory::addChild: attempt to fit " + std::to_string(m_instanceCount+1) +
        " children in a factory with size " + std::to_string(m_count));
    }

    instance.m_factoryOffset = m_instanceCount * sizeof(RectangleUniform);
    instance.m_uniform = &m_ubos[m_instanceCount];
    instance.m_uboBuffer = &m_uboBuffer;
    m_totalSize += sizeof(RectangleUniform);

    m_instanceCount++;
}

void RectangleFactory::create(View& view, uint32_t count, bool colorBlending) {
    m_count = count;
    m_ubos.resize(count);

    ObjectCreateInfo createInfo;
    createInfo.colorBlending = colorBlending;

    core::BufferCreateInfo bufferInfo = {};
    bufferInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    bufferInfo.size = sizeof(RectangleUniform) * count;
    bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    m_uboBuffer.create(bufferInfo);

    createInfo.depthTest = false;
    // createInfo.descriptor.sets.push_back(view.getCamera().m_descriptor.set);
    // createInfo.descriptor.layouts.push_back(view.getCamera().m_descriptor.layout);
    createInfo.descriptors.push_back(view.getCamera().getDescriptor());

    createInfo.index.buffer = core::createDeviceLocalBuffer(indicies.data(), indicies.size() * sizeof(indicies[0]), VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
    createInfo.index.count = indicies.size();
    createInfo.index.type = VK_INDEX_TYPE_UINT16;
    createInfo.minSampleShading = 0.0f;
    createInfo.vertex.attributeDescriptions = Vertex::getAttributeDescriptions();
    createInfo.vertex.bindingDescription = Vertex::getBindingDescription();
    createInfo.vertex.buffer = core::createDeviceLocalBuffer(verticies.data(), verticies.size() * sizeof(Vertex), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    createInfo.viewport = view.getViewport();
    createInfo.instanceCount = 0;

    createInfo.descriptors.push_back(getDescriptor(m_uboBuffer));
    createInfo.shaders = createFactoryShaders(count);

    createObject(createInfo);

    createInfo.shaders.destroy();
}

void RectangleFactory::destroy() {
    m_uboBuffer.destroy();
    freeDescriptor(m_setPools[1], m_descriptorSets[1]);
    m_vertex.buffer.destroy();
    m_index.buffer.destroy();
    vkDestroyPipeline(core::apiCore.device, m_pipeline, nullptr);
}

void RectangleFactory::upload() {
    m_uboBuffer.loadData(m_ubos.data(), m_totalSize);
}

void TexturedRectangleFactory::addChild(TexturedRectangleInstance& instance) {
    if (m_instanceCount >= m_count) {
        throw std::runtime_error("RectangleFactory::addChild: attempt to fit " + std::to_string(m_instanceCount+1) +
        " children in a factory with size " + std::to_string(m_count));
    }

    instance.m_factoryOffset = m_instanceCount * sizeof(TexturedRectangleUniform);
    instance.m_uniform = &m_ubos[m_instanceCount];
    instance.m_uboBuffer = &m_uboBuffer;
    m_totalSize += sizeof(TexturedRectangleUniform);

    m_instanceCount++;
}

void TexturedRectangleFactory::create(View& view, uint32_t count, Texture& texture, bool colorBlending) {
    m_count = count;
    m_ubos.resize(count);

    ObjectCreateInfo createInfo;
    createInfo.colorBlending = colorBlending;

    core::BufferCreateInfo bufferInfo = {};
    bufferInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    bufferInfo.size = sizeof(TexturedRectangleUniform) * count;
    bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    m_uboBuffer.create(bufferInfo);

    createInfo.depthTest = false;
    // createInfo.descriptor.sets.push_back(view.getCamera().m_descriptor.set);
    // createInfo.descriptor.layouts.push_back(view.getCamera().m_descriptor.layout);
    createInfo.descriptors.push_back(view.getCamera().getDescriptor());

    createInfo.index.buffer = core::createDeviceLocalBuffer(indicies.data(), indicies.size() * sizeof(indicies[0]), VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
    createInfo.index.count = indicies.size();
    createInfo.index.type = VK_INDEX_TYPE_UINT16;
    createInfo.minSampleShading = 0.0f;
    createInfo.vertex.attributeDescriptions = Vertex::getAttributeDescriptions();
    createInfo.vertex.bindingDescription = Vertex::getBindingDescription();
    createInfo.vertex.buffer = core::createDeviceLocalBuffer(verticies.data(), verticies.size() * sizeof(Vertex), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    createInfo.viewport = view.getViewport();
    createInfo.instanceCount = 0;

    createInfo.descriptors.push_back(getTDescriptor(m_uboBuffer, sizeof(TexturedRectangleUniform), texture));
    // createInfo.descriptor.sets.push_back(d.getSet());
    // createInfo.descriptor.layouts.push_back(d.getLayout());
    createInfo.shaders = createTexturedFactoryShaders(count);

    createObject(createInfo);

    createInfo.shaders.destroy();
}

void TexturedRectangleFactory::destroy() {
    m_uboBuffer.destroy();
    freeDescriptor(m_setPools[1], m_descriptorSets[1]);
    m_vertex.buffer.destroy();
    m_index.buffer.destroy();
    vkDestroyPipeline(core::apiCore.device, m_pipeline, nullptr);
}

void TexturedRectangleFactory::upload() {
    m_uboBuffer.loadData(m_ubos.data(), m_totalSize);
}

void RectangleInstance::setColor(const glm::vec4& color) {
    m_uniform->color = color;
}

void RectangleInstance::setColor(float r, float g, float b, float a) {
    m_uniform->color.r = r;
    m_uniform->color.g = g;
    m_uniform->color.b = b;
    m_uniform->color.a = a;
}

void RectangleInstance::updateTransform() {
    m_uniform->transform = getTransform();
}

// FIXME
void RectangleInstance::upload() {
    m_uboBuffer->loadData(m_uniform, sizeof(RectangleUniform), m_factoryOffset);
}

void TexturedRectangleInstance::setTexCoords(glm::vec2 coords[4]) {
    for (size_t i = 0; i < 4; ++i) {
        m_uniform->texCoords[i] = coords[i];
    }
}

void TexturedRectangleInstance::updateTransform() {
    m_uniform->transform = getTransform();
}

// FIXME
void TexturedRectangleInstance::upload() {
    m_uboBuffer->loadData(m_uniform, sizeof(TexturedRectangleUniform), m_factoryOffset);
}

} // namespace age
