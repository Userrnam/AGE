#pragma once

namespace age {

namespace vk {

typedef struct Buffer_T* Buffer;
typedef struct Descriptor_T* Descriptor;
typedef struct Pipeline_T* Pipeline;
    
} // namespace vk

// 
struct ObjectCreateInfo {

};

class Object {
private:
    vk::Pipeline pipeline;
    vk::Buffer vertexBuffer = nullptr;
    vk::Buffer indexBuffer = nullptr;
    vk::Descriptor descriptor = nullptr; // ubo + sampler
    bool useCameraDescriptor = true;
    uint32_t indexCount = 0;
public:
    void create(ObjectCreateInfo& info);

    void draw(int i);
};

} // namespace age
