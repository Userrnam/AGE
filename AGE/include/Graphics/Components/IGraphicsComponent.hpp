#pragma once

#include <variant>
#include <string>
#include <vulkan/vulkan.h>

#include "../Buffer.hpp"
#include "../Texture.hpp"

namespace age {

struct GraphicsComponentDescription {
    VkShaderStageFlags stage;
    VkDescriptorType type;
    std::variant<Buffer*, Texture*> descriptor;
};

// this is used to generate shaders and descriptors
struct IGraphicsComponent {
    // insert in global scope in vertex shader
    // increment location if was used
    virtual std::string getVertStartInsert(int binding, int& outLocatin) = 0;
    // insert in main in vertex shader
    virtual std::string getVertEndInsert() = 0;

    // insert in global scope in fragment shader
    // increment location if was used
    virtual std::string getFragStartInsert(int binding, int& inLocation) = 0;
    // insert in main in fragment shader
    virtual std::string getFragEndInsert() = 0;

    // used in descriptorSet
    virtual GraphicsComponentDescription getDescription() = 0;
};

/*
    virtual std::string getVertStartInsert(int binding, int& outLocatin) override {

    }
    
    virtual std::string getVertEndInsert() override {

    }
    
    virtual std::string getFragStartInsert(int binding, int& inLocation) override {

    }
    
    virtual std::string getFragEndInsert() override {

    }

    virtual GraphicsComponentDescription getDescription() override {

    }
*/

} // namespace age
