#pragma once
#include "vector.hpp"
#include "matrix.hpp"

namespace {
    constexpr float pi = 3.14159265359f;
}

static float wrapAngle(float angle) {
    while (angle > pi) {
        angle -= pi * 2;
    }
    while (angle < -pi) {
        angle += pi * 2;
    }
    return angle;
}

static float sign(float x) {
    return x > 0.0f ? 1.0f : x < 0.0f ? -1.0f : 0.0f;
}

template <class T>
T lerp(const T& a, const T& b, float x) {
    return a * (1 - x) + b * x;
}

template <class T>
T clamp(const T& x, const T& min, const T& max) {
    return x > max ? max : x < min ? min : x;
}
