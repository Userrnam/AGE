#pragma once

#include <iostream>
#include <initializer_list>

namespace age {


template<typename T, int size, T(*rootFunc)(T)>
class FPVector;

template<typename T, int size>
class Vector {
    T m_data[size];
public:
    // constructors

    Vector() {}

    Vector(T data[size]) {
        for (size_t i = 0; i < size; ++i) {
            m_data[i] = data[i];
        }
    }

    Vector(std::initializer_list<T> data) {
        for (size_t i = 0; i < size; ++i) {
            m_data[i] = data.begin()[i];
        }
    }

    // operators

    template<T(*rootFunc)(T)>
    operator FPVector<T, size, rootFunc>() const {
        return *(FPVector<T, size, rootFunc>*)this;
    }

    Vector& operator*=(T c) {
        for (size_t i = 0; i < size; ++i) {
            m_data[i] *= c;
        }
        return *this;
    }

    Vector& operator+=(const Vector& rhs) {
        for (size_t i = 0; i < size; ++i) {
            m_data[i] += rhs.m_data[i];
        }
        return *this;
    }

    Vector operator+(const Vector& rhs) {
        Vector out = *this;
        out += rhs;
        return out;
    }

    Vector& operator-=(const Vector& rhs) {
        for (size_t i = 0; i < size; ++i) {
            m_data[i] -= rhs.m_data[i];
        }
        return *this;
    }

    Vector operator-(const Vector& rhs) {
        Vector out = *this;
        out -= rhs;
        return out;
    }

    Vector operator-() {
        Vector out = {};
        out -= *this;
        return out;
    }

    T& operator[](size_t i) {
        return m_data[i];
    }

    const T& operator[](size_t i) const {
        return m_data[i];
    }

    friend Vector operator*(T c, const Vector& rhs) {
        Vector out = rhs;
        out *= c;
        return *out;
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector& v) {
        for (size_t i = 0; i < size; ++i) {
            os << v.m_data[i] << '\n';
        }
        return os;
    }
};

template<typename T, int size, T(*rootFunc)(T)>
class FPVector : public Vector<T, size> {
public:
    FPVector(std::initializer_list<T> data) : Vector<T, size>(data) {}
    FPVector() {}

    T length() {
        T len;
        for (size_t i = 0; i < size; ++i) {
            len += (*this)[i] * (*this)[i];
        }

        return rootFunc(len);
    }

    void normilize() {
        T len = this->length();

        for (size_t i = 0; i < size; ++i) {
            (*this)[i] /= len;
        }
    }
};

typedef FPVector<float, 4, sqrtf> Vector4f;
typedef FPVector<float, 3, sqrtf> Vector3f;
typedef FPVector<float, 2, sqrtf> Vector2f;

typedef Vector<int, 4> Vector4i;
typedef Vector<int, 3> Vector3i;
typedef Vector<int, 2> Vector2i;

} // namespace age
