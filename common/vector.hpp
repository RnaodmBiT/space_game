#pragma once
#include "vector_base.hpp"
#include <cmath>


template <class T, int N>
class vector : public Vector_data<T, N> {
public:

    vector(T x = 0) {
        for (int i = 0; i < N; ++i) {
            this->data[i] = x;
        }
    }

    vector(std::initializer_list<T> elements) {
        int i = 0;
        for (const T& e : elements) {
            this->data[i++] = e;
        }
    }

    vector(const vector& v) {
        for (int i = 0; i < N; ++i) {
            this->data[i] = v.data[i];
        }
    }


    T dot(const vector& v) const {
        T sum = 0;
        for (int i = 0; i < N; ++i) {
            sum += this->data[i] * v.data[i];
        }
        return sum;
    }


    template <class U = T>
    typename std::enable_if<N == 3, vector<U, 3>>::type cross(const vector& v) const {
        return vector{
            this->y * v.z - this->z * v.y,
            this->z * v.x - this->x * v.z,
            this->x * v.y - this->y * v.x
        };
    }

    template <class U = T>
    typename std::enable_if<N == 2, U>::type cross(const vector& v) const {
        return this->x * v.y - this->y * v.x;
    }


    T lengthSquared() const {
        T sum = 0;
        for (int i = 0; i < N; ++i) {
            sum += this->data[i] * this->data[i];
        }
        return sum;
    }


    T length() const {
        return std::sqrt(lengthSquared());
    }


    void normalize() {
        T l = length();
        for (int i = 0; i < N; ++i) {
            this->data[i] /= l;
        }
    }


    vector normalized() const {
        vector v = *this;
        v.normalize();
        return v;
    }


    vector operator-() const {
        vector negate;
        for (int i = 0; i < N; ++i) {
            negate.data[i] = -this->data[i];
        }
        return negate;
    }


    vector& operator=(const vector& v) {
        for (int i = 0; i < N; ++i) {
            this->data[i] = v.data[i];
        }
        return *this;
    }

    vector& operator+=(const vector& v) {
        for (int i = 0; i < N; ++i) {
            this->data[i] += v.data[i];
        }
        return *this;
    }

    vector& operator-=(const vector& v) {
        for (int i = 0; i < N; ++i) {
            this->data[i] -= v.data[i];
        }
        return *this;
    }

    vector& operator*=(const vector& v) {
        for (int i = 0; i < N; ++i) {
            this->data[i] *= v.data[i];
        }
        return *this;
    }

    vector& operator/=(const vector& v) {
        for (int i = 0; i < N; ++i) {
            this->data[i] /= v.data[i];
        }
        return *this;
    }

    vector& operator+=(const T& v) {
        for (int i = 0; i < N; ++i) {
            this->data[i] += v;
        }
        return *this;
    }

    vector& operator-=(const T& v) {
        for (int i = 0; i < N; ++i) {
            this->data[i] -= v;
        }
        return *this;
    }

    vector& operator*=(const T& v) {
        for (int i = 0; i < N; ++i) {
            this->data[i] *= v;
        }
        return *this;
    }

    vector& operator/=(const T& v) {
        for (int i = 0; i < N; ++i) {
            this->data[i] /= v;
        }
        return *this;
    }

};


template <class T, int N>
vector<T, N> operator+ (vector<T, N> a, const vector<T, N>& b) {
    return a += b;
}

template <class T, int N>
vector<T, N> operator- (vector<T, N> a, const vector<T, N>& b) {
    return a -= b;
}

template <class T, int N>
vector<T, N> operator* (vector<T, N> a, const vector<T, N>& b) {
    return a *= b;
}

template <class T, int N>
vector<T, N> operator/ (vector<T, N> a, const vector<T, N>& b) {
    return a /= b;
}

template <class T, int N>
vector<T, N> operator+ (vector<T, N> a, const T& b) {
    return a += b;
}

template <class T, int N>
vector<T, N> operator- (vector<T, N> a, const T& b) {
    return a -= b;
}

template <class T, int N>
vector<T, N> operator* (vector<T, N> a, const T& b) {
    return a *= b;
}

template <class T, int N>
vector<T, N> operator/ (vector<T, N> a, const T& b) {
    return a /= b;
}


typedef vector<float, 2> vec2;
typedef vector<float, 3> vec3;
typedef vector<float, 4> vec4;

typedef vector<double, 2> vec2d;
typedef vector<double, 3> vec3d;
typedef vector<double, 4> vec4d;

typedef vector<int, 2> vec2i;
typedef vector<int, 3> vec3i;
typedef vector<int, 4> vec4i;

static float angleBetween(const vec2& to, const vec2& from) {
    return std::atan2(to.y - from.y, to.x - from.x);
}
