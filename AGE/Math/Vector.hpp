#pragma once

#include <ostream>

namespace age {

template<typename T>
struct Vector2 {
    T x = {};
    T y = {};

    Vector2() {}
    constexpr Vector2(T _x, T _y) { x = _x; y = _y; }
    Vector2(T f) { x = f; y = f; }

    Vector2& operator+=(const Vector2& rhs) {
        x += rhs.x;
        y += rhs.y;

        return *this;
    }

    Vector2& operator-=(const Vector2& rhs) {
        x -= rhs.x;
        y -= rhs.y;

        return *this;
    }

    Vector2& operator*=(const Vector2& rhs) {
        x *= rhs.x;
        y *= rhs.y;

        return *this;
    }

    Vector2& operator/=(const Vector2& rhs) {
        x /= rhs.x;
        y /= rhs.y;

        return *this;
    }

    Vector2 operator+(const Vector2& rhs) const {
        Vector2 res = *this;
        res += rhs;

        return res;
    }

    Vector2 operator-(const Vector2& rhs) const {
        Vector2 res = *this;
        res -= rhs;

        return res;
    }

    Vector2 operator*(const Vector2& rhs) const {
        Vector2 res = *this;
        res *= rhs;

        return res;
    }

    Vector2 operator/(const Vector2& rhs) const {
        Vector2 res = *this;
        res /= rhs;

        return res;
    }
};

template<typename T>
inline Vector2<T> operator*(T lhs, const Vector2<T>& rhs) {
    auto res = rhs;

    res.x *= lhs;
    res.y *= lhs;

    return res;
}

template<typename T>
struct Vector3 {
    T x = {};
    T y = {};
    T z = {};

    Vector3() {}
    constexpr Vector3(T _x, T _y, T _z) { x = _x; y = _y; z = _z; }
    Vector3(T f) { x = f; y = f; z = f; }

    Vector3& operator+=(const Vector3& rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;

        return *this;
    }

    Vector3& operator-=(const Vector3& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;

        return *this;
    }

    Vector3& operator*=(const Vector3& rhs) {
        x *= rhs.x;
        y *= rhs.y;
        z *= rhs.z;

        return *this;
    }

    Vector3& operator/=(const Vector3& rhs) {
        x /= rhs.x;
        y /= rhs.y;
        z /= rhs.z;

        return *this;
    }

    Vector3 operator+(const Vector3& rhs) const {
        Vector3 res = *this;
        res += rhs;

        return res;
    }

    Vector3 operator-(const Vector3& rhs) const {
        Vector3 res = *this;
        res -= rhs;

        return res;
    }

    Vector3 operator*(const Vector3& rhs) const {
        Vector3 res = *this;
        res *= rhs;

        return res;
    }

    Vector3 operator/(const Vector3& rhs) const {
        Vector3 res = *this;
        res /= rhs;

        return res;
    }
};

template<typename T>
inline Vector3<T> operator*(T lhs, const Vector3<T>& rhs) {
    auto res = rhs;

    res.x *= lhs;
    res.y *= lhs;
    res.z *= lhs;

    return res;
}

template<typename T>
struct Vector4 {
    T x = {};
    T y = {};
    T z = {};
    T w = {};

    Vector4() {}
    constexpr Vector4(T _x, T _y, T _z, T _w) { x = _x; y = _y; z = _z; w = _w; }
    Vector4(T f) { x = f; y = f; z = f; w = f; }

    Vector4& operator+=(const Vector4& rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;

        return *this;
    }

    Vector4& operator-=(const Vector4& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        w -= rhs.w;

        return *this;
    }

    Vector4& operator*=(const Vector4& rhs) {
        x *= rhs.x;
        y *= rhs.y;
        z *= rhs.z;
        w *= rhs.w;

        return *this;
    }

    Vector4& operator/=(const Vector4& rhs) {
        x /= rhs.x;
        y /= rhs.y;
        z /= rhs.z;
        w /= rhs.w;

        return *this;
    }

    Vector4 operator+(const Vector4& rhs) const {
        Vector4 res = *this;
        res += rhs;

        return res;
    }

    Vector4 operator-(const Vector4& rhs) const {
        Vector4 res = *this;
        res -= rhs;

        return res;
    }

    Vector4 operator*(const Vector4& rhs) const {
        Vector4 res = *this;
        res *= rhs;

        return res;
    }

    Vector4 operator/(const Vector4& rhs) const {
        Vector4 res = *this;
        res /= rhs;

        return res;
    }
};

template<typename T>
inline Vector4<T> operator*(T lhs, const Vector4<T>& rhs) {
    auto res = rhs;

    res.x *= lhs;
    res.y *= lhs;
    res.z *= lhs;
    res.w *= lhs;

    return res;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Vector2<T>& v) {
    os << v.x << "; " << v.y;
    return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Vector3<T>& v) {
    os << v.x << "; " << v.y << "; " << v.z;
    return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Vector4<T>& v) {
    os << v.x << "; " << v.y << "; " << v.z << "; " << v.w;
    return os;
}

typedef Vector2<float> Vector2f;
typedef Vector3<float> Vector3f;
typedef Vector4<float> Vector4f;

typedef Vector2<int> Vector2i;
typedef Vector3<int> Vector3i;
typedef Vector4<int> Vector4i;

} // namespace age
