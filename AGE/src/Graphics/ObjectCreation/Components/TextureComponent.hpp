#pragma once

#include <sstream>
#include <glm/glm.hpp>
#include <vector>

#include "ShaderComponent.hpp"

#include "Objects/Texture.hpp"

namespace age {

// ! requires vec2 texCoords in fragment shader
class TextureComponent {
    Texture m_texture;

protected:
    inline Texture getTexture() { return m_texture; }

public:
    inline void setTexture(const Texture& texture) {
        m_texture = texture;
    }

    TextureComponent() {}
    inline TextureComponent(const Texture& texture) { m_texture = texture; }

    ShaderComponentInfo __getInfo() {
        ShaderComponentInfo info;

        info.add(
            ShaderComponentTexture()
            .setName("textureSampler")
            .setFragMainInsert("\tfragColor *= texture(textureSampler, globals.texCoords);\n")
        );

        return info;
    }

    ShaderComponentInfo getInfo() {
        ShaderComponentInfo info;

        info.add(
            ShaderComponentTexture()
            .setName("textureSampler")
            .setTexture(m_texture)
            .setFragMainInsert("\tfragColor *= texture(textureSampler, globals.texCoords);\n")
        );
        info.setId<TextureComponent>();

        return info;
    }
};

} // namespace age
