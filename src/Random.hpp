#pragma once

#include <cstdint>
#include <limits>
#include <iostream>
#include "unstd/unstdlib.hpp"
#include <cstdlib>

template <typename T>
T uniform_random(T min, T max) {
    static_assert(sizeof(T) == 0 && "uniform_random not implemented for type!");
}

template <> f32 uniform_random<f32>(f32 min, f32 max);
