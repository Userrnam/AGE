#pragma once

#include <sstream>
#include <glm/glm.hpp>
#include <vector>

#include "ShaderComponent.hpp"

#include "../Texture.hpp"

namespace age {

// ! requires vec2 texCoords in fragment shader
class TextureComponent {
    Texture m_texture;

public:
    void create(std::string path, Shared<Sampler> sampler)  {
        m_texture.create(path, sampler);
    }

    void destroy() {
        m_texture.destroy();
    }

    ShaderComponentInfo getInfo() {
        ShaderComponentInfo info;
        info.setFragInsert(
            ShaderComponentInsert()
            .addLayout(
                Layout()
                .setName("textureSampler")
                .setType("uniform sampler2D", LayoutType::SAMPLER)
            )
            .setMainInsert("\tfragColor *= texture(textureSampler, globals.texCoords);\n")
        );
        info.setDescription(
            ShaderComponentDescription()
            .setStage(VK_SHADER_STAGE_FRAGMENT_BIT)
            .setType(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
            .setTexture(m_texture)
        );
        return info;
    }
};

} // namespace age
