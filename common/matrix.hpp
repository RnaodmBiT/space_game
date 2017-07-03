#pragma once
#include <array>
#include "vector.hpp"

template <class T, int Rows, int Cols>
class matrix {
public:
    static const int Size = Rows * Cols;
    std::array<T, Size> data;

    matrix() {
        for (int y = 0; y < Rows; ++y) {
            for (int x = 0; x < Cols; ++x) {
                at(y, x) = (x == y ? T(1) : T(0));
            }
        }
    }

    template <int X, int Y>
    matrix(const matrix<T, X, Y>& other) {
        static_assert(X <= Rows, "Matrix must be of smaller dimensions");
        static_assert(Y <= Cols, "Matrix must be of smaller dimensions");
        for (int y = 0; y < Rows; ++y) {
            for (int x = 0; x < Cols; ++x) {
                if (x < X && y < Y) {
                    at(y, x) = other(y, x);
                } else {
                    at(y, x) = (x == y ? T(1) : T(0));
                }
            }
        }
    }

    void operator=(const matrix& other) {
        for (int y = 0; y < Rows; ++y) {
            for (int x = 0; x < Cols; ++x) {
                at(y, x) = other(y, x);
            }
        }
    }

    void operator*=(const matrix& other) {
        static_assert(Rows == Cols, "Can only multiply-assign two matrices of the same dimensions.");
        matrix result;
        for (int row = 0; row < Rows; ++row) {
            for (int col = 0; col < Cols; ++col) {
                result(row, col) = T(0);
                for (int i = 0; i < Cols; ++i) {
                    result(row, col) += at(row, i) * other(i, col);
                }
            }
        }

        *this = result;
    }

    T& at(int row, int col) {
        return data[col + row * Cols];
    }

    const T& at(int row, int col) const {
        return data[col + row * Cols];
    }

    T& operator()(int row, int col) {
        return at(row, col);
    }

    const T& operator()(int row, int col) const {
        return at(row, col);
    }

};


template <class T, int Rows, int Inner, int Cols>
matrix<T, Rows, Cols> operator*(const matrix<T, Rows, Inner>& left, const matrix<T, Inner, Cols>& other) {
    matrix<T, Rows, Cols> result;
    for (int row = 0; row < Rows; ++row) {
        for (int col = 0; col < Cols; ++col) {
            result(row, col) = T(0);
            for (int i = 0; i < Inner; ++i) {
                result(row, col) += left(row, i) * other(i, col);
            }
        }
    }
    return result;
}

template <class T, int Rows, int Cols>
vector<T, Rows> operator*(const matrix<T, Rows, Cols>& left, const vector<T, Cols>& right) {
    vector<T, Rows> result;
    for (int i = 0; i < Rows; ++i) {
        result.data[i] = T(0);
        for (int j = 0; j < Cols; ++j) {
            result.data[i] += left(i, j) * right.data[j];
        }
    }
    return result;
}

typedef matrix<float, 4, 4> mat4;
typedef matrix<float, 3, 3> mat3;


static mat4 orthographic(float left, float top, float right, float bottom, float n = 0.0f, float f = 1.0f) {
    mat4 ortho;
    ortho(0, 0) = 2.0f / (right - left);
    ortho(0, 3) = -(right + left) / (right - left);
    ortho(1, 1) = 2.0f / (top - bottom);
    ortho(1, 3) = -(top + bottom) / (top - bottom);
    ortho(2, 2) = -2.0f / (f - n);
    ortho(2, 3) = -(f + n) / (f - n);
    return ortho;
}

static mat4 perspective(float fov, float aspect, float n, float f) {
    float t = std::tan(fov / 2);
    mat4 m;
    m.at(0, 0) = 1 / (aspect * t);
    m.at(1, 1) = 1 / t;
    m.at(2, 2) = (f) / (f - n);
    m.at(2, 3) = -(f * n) / (f - n);
    m.at(3, 2) = 1;
    m.at(3, 3) = 0;
    return m;
}


static mat4 look_at(vec3 position, vec3 target, vec3 up) {
    vec3 z = (target - position).normalized();
    vec3 x = up.cross(z).normalized();
    vec3 y = z.cross(x).normalized();

    mat4 m;
    m.at(0, 0) = x.x;
    m.at(0, 1) = x.y;
    m.at(0, 2) = x.z;
    m.at(0, 3) = -x.dot(position);

    m.at(1, 0) = y.x;
    m.at(1, 1) = y.y;
    m.at(1, 2) = y.z;
    m.at(1, 3) = -y.dot(position);

    m.at(2, 0) = z.x;
    m.at(2, 1) = z.y;
    m.at(2, 2) = z.z;
    m.at(2, 3) = -z.dot(position);

    return m;
}

template <class T>
matrix<T, 4, 4> translate(T x, T y, T z) {
    matrix<T, 4, 4> ret;
    ret(0, 3) = x;
    ret(1, 3) = y;
    ret(2, 3) = z;
    return ret;
}

template <class T>
matrix<T, 4, 4> translate(const vector<T, 3>& vec) {
    return translate(vec.x, vec.y, vec.z);
}

template <class T>
matrix<T, 4, 4> translate(const vector<T, 2>& vec) {
    return translate(vec.x, vec.y, T(0));
}

template <class T>
matrix<T, 4, 4> scale(T x, T y, T z) {
    matrix<T, 4, 4> ret;
    ret(0, 0) = x;
    ret(1, 1) = y;
    ret(2, 2) = z;
    return ret;
}

template <class T>
matrix<T, 4, 4> scale(const vector<T, 3>& vec) {
    return scale(vec.x, vec.y, vec.z);
}

template <class T>
matrix<T, 4, 4> scale(const vector<T, 2>& vec) {
    return scale(vec.x, vec.y, T(1));
}

static mat4 rotate(float angle, const vec3& axis) {
    mat4 mat;
    float c = std::cos(angle);
    float s = std::sin(angle);

    mat.at(0, 0) = c + axis.x * axis.x * (1 - c);
    mat.at(0, 1) = axis.x * axis.y * (1 - c) - axis.z * s;
    mat.at(0, 2) = axis.x * axis.z * (1 - c) + axis.y * s;

    mat.at(1, 0) = axis.x * axis.y * (1 - c) + axis.z * s;
    mat.at(1, 1) = c + axis.y * axis.y * (1 - c);
    mat.at(1, 2) = axis.y * axis.z * (1 - c) + axis.x * s;

    mat.at(2, 0) = axis.x * axis.z * (1 - c) - axis.y * s;
    mat.at(2, 1) = axis.y * axis.z * (1 - c) + axis.x * s;
    mat.at(2, 2) = c + axis.z * axis.z * (1 - c);

    return mat;
}
