#include "Random.hpp"

template <>
f32 uniform_random<f32>(f32 min, f32 max) {
    f32 r = static_cast<f32>(rand()) / static_cast<f32>(RAND_MAX);
    return min + r * (max - min);
}

