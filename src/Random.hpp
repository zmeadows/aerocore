#pragma once

#include <cstdint>
#include <limits>

//period 2^96-1
inline u32 xorshf96(void) {
    static u32 x=123456789, y=362436069, z=521288629;
    u32 t;
    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;

    t = x;
    x = y;
    y = z;
    z = t ^ x ^ y;

    return z;
}

inline f32 uniform_random(f32 min_, f32 max_) {
    const f32 r = static_cast<f32>(xorshf96());
    const f32 d = static_cast<f32>(std::numeric_limits<uint_fast32_t>::max());
    return min_ + (max_ - min_) * r / d;
}

inline bool coin_flip(void) {
    return uniform_random(0,1) > 0.5;
}
