#pragma once

#include <sstream>
#include <glm/glm.hpp>
#include <vector>

#include "IGraphicsComponent.hpp"

#include "../Texture.hpp"

namespace age {

// ! requires vec2 texCoords in fragment shader
class TextureComponent : public IGraphicsComponent {
    Texture texture;

public:
    void create(std::string path, Shared<Sampler> sampler)  {
        texture.create(path, sampler);
    }

    void destroy() {
        texture.destroy();
    }

// Interface
    virtual std::vector<Layout> getFragLayouts() override {
        std::vector<Layout> layouts;
        layouts.push_back(
            Layout()
            .setName("textureSampler")
            .setType("uniform sampler2D", LayoutType::SAMPLER)
        );
        return layouts;
    }

    virtual std::string getFragMainInsert() override {
        return "fragColor *= texture(textureSampler, texCoords);\n";
    }

    virtual GraphicsComponentDescription getDescription() override {
        GraphicsComponentDescription description;
        description.m_stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        description.m_type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        description.m_descriptor = &texture;

        return description;
    }
};

} // namespace age
