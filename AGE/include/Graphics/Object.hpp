#pragma once

#include <string>
#include <vector>

#include "Core/Vertex.hpp"
#include "Shader.hpp"

// FIXME
#include "Core/Buffer.hpp"

namespace age {

namespace vk {

typedef struct Buffer_T* Buffer;
typedef struct Descriptor_T* Descriptor;
typedef struct Pipeline_T* Pipeline;
    
} // namespace vk

struct ObjectCreateInfo {
    core::Vertex* vertex = nullptr;
    std::vector<Shader> shaders;
    bool depthTestEnable = true;
    bool requiresCamera = true;
    std::vector<VkDescriptorSetLayout> layouts;
};

class Object {
private:
    vk::Pipeline pipeline;
    core::Buffer vertexBuffer;
    core::Buffer indexBuffer;
    vk::Descriptor descriptor = nullptr; // ubo + sampler
    bool useCameraDescriptor = true;
    uint32_t indexCount = 0;
public:
    void create(ObjectCreateInfo& info);

    void draw(int i);
};

} // namespace age
