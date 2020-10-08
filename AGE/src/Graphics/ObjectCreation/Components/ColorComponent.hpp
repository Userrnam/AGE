#pragma once

#include <sstream>
#include <glm/glm.hpp>

#include "ShaderComponent.hpp"
#include "GetSet.hpp"

namespace age {

class Color : public util::GetSet<Vector4f> {
public:
    static ShaderComponentInfo __getInfo() {
        ShaderComponentInfo info;
        info.add(
            ShaderComponentBuffer()
            .addBlockMember("vec4 color", true)
            .setFragMainInsert("\tfragColor *= globals.color;\n")
        );
        return info;
    }

    inline Color(Vector4f v = Vector4f(1)) { set(v); }
    inline Color(float r, float g, float b, float a = 1.0f) { set({r, g, b, a}); }

    inline Color& operator+=(const Color& rhs) {
        get() += rhs.get();
        return *this;
    }

    inline Color& operator-=(const Color& rhs) {
        get() -= rhs.get();
        return *this;
    }

    inline Color operator+(const Color& rhs) {
        return get() + rhs.get();
    }

    inline Color operator-(const Color& rhs) {
        return get() + rhs.get();
    }
};

inline Color operator*(float x, const Color& rhs) {
    return x * rhs.get();
}

} // namespace age
