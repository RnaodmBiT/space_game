#pragma once
#include <array>



template <class T, int N>
class Vector_data {
public:
    std::array<T, N> data;
};


// Common 3D vector specializations
template <class T>
class Vector_data<T, 2> {
public:
    union {
        std::array<T, 2> data;
        struct { T x, y; };
    };
};


template <class T>
class Vector_data<T, 3> {
public:
    union {
        std::array<T, 3> data;
        struct { T x, y, z; };
    };
};


template <class T>
class Vector_data<T, 4> {
public:
    union {
        std::array<T, 4> data;
        struct { T x, y, z, w; };
    };
};
