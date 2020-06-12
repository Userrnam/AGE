#pragma once

#include "Vector.hpp"

namespace age {

namespace detail {

template<typename T>
inline void matMul(T* out, const T* lhs, const T* rhs, size_t lrows, size_t lcols, size_t rcols) {
    for (size_t i = 0; i < lrows; ++i) {
        for (size_t j = 0; j < rcols; ++j) {
            for (size_t k = 0; k < lcols; ++k) {
                out[j * lrows + i] += lhs[k * lrows + i] * rhs[j * lcols + k];
            }
        }
    }
}

} // namespace detail

template<typename T, int rows, int cols>
class Matrix {
    Vector<T, rows> m_data[cols] = {};

public:
    Matrix() {}

    Matrix(std::initializer_list<Vector<T, rows>> data) {
        for (size_t i = 0; i < cols; ++i) {
            m_data[i] = data.begin()[i];
        }
    }

    Matrix(std::initializer_list<T> data) {
        size_t k = 0;
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                m_data[j][i] = data.begin()[k];
                k++;
            }
        }
    }

    Matrix& operator*=(T c) {
        for (size_t i = 0; i < cols; ++i) {
            m_data[i] *= c;
        }
        return *this;
    }

    Matrix& operator+=(const Matrix& rhs) {
        for (size_t i = 0; i < rows; ++i) {
            m_data[i] += rhs.m_data[i];
        }
        return *this;
    }

    Matrix operator+(const Matrix& rhs) {
        Matrix out = *this;
        out += rhs;
        return out;
    }

    Matrix& operator-=(const Matrix& rhs) {
        for (size_t i = 0; i < rows; ++i) {
            m_data[i] -= rhs.m_data[i];
        }
        return *this;
    }

    Matrix operator-(const Matrix& rhs) {
        Matrix out = *this;
        out -= rhs;
        return out;
    }

    Matrix operator-() {
        Matrix out = {};
        out -= *this;
        return out;
    }

    friend Matrix operator*(T c, const Matrix& rhs) {
        Matrix out = rhs;
        out *= c;
        return out;
    }

    template<int rcols>
    friend Matrix<T, rows, rcols> operator*(const Matrix<T, rows, cols>& lhs, const Matrix<T, cols, rcols>& rhs) {
        Matrix<T, rows, rcols> out;
        detail::matMul<T>((T*)&out, (const T*)&lhs, (const T*)&rhs, rows, cols, rcols);
        return out;
    }

    friend Vector<T, cols> operator*(const Matrix<T, rows, cols>& lhs, const Vector<T, cols>& rhs) {
        Vector<T, cols> out;
        detail::matMul<T>((T*)&out, (const T*)&lhs, (const T*)&rhs, rows, cols, 1);
        return out;
    }

    Vector<T, rows>& operator[](size_t i) {
        return m_data[i];
    }

    const Vector<T, rows>& operator[](size_t i) const {
        return m_data[i];
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
        for (size_t j = 0; j < rows; ++j) {
            for (size_t i = 0; i < cols; ++i) {
                os << m.m_data[i][j] << ' ';
            }
            os << '\n';
        }
        return os;
    }
};

typedef Matrix<float, 4, 4> Matrix4x4f;
typedef Matrix<float, 4, 3> Matrix4x3f;
typedef Matrix<float, 4, 2> Matrix4x2f;
typedef Matrix<float, 3, 4> Matrix3x4f;
typedef Matrix<float, 3, 3> Matrix3x3f;
typedef Matrix<float, 3, 2> Matrix3x2f;
typedef Matrix<float, 2, 4> Matrix2x4f;
typedef Matrix<float, 2, 3> Matrix2x3f;
typedef Matrix<float, 2, 2> Matrix2x2f;

typedef Matrix<int, 4, 4> Matrix4x4i;
typedef Matrix<int, 4, 3> Matrix4x3i;
typedef Matrix<int, 4, 2> Matrix4x2i;
typedef Matrix<int, 3, 4> Matrix3x4i;
typedef Matrix<int, 3, 3> Matrix3x3i;
typedef Matrix<int, 3, 2> Matrix3x2i;
typedef Matrix<int, 2, 4> Matrix2x4i;
typedef Matrix<int, 2, 3> Matrix2x3i;
typedef Matrix<int, 2, 2> Matrix2x2i;

typedef Matrix4x4f Matrix4f;
typedef Matrix3x3f Matrix3f;
typedef Matrix2x2f Matrix2f;

typedef Matrix4x4i Matrix4i;
typedef Matrix3x3i Matrix3i;
typedef Matrix2x2i Matrix2i;

} // namespace age
