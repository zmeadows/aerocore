#pragma once

#include "Vector2D.hpp"
#include "Typedef.hpp"

#include <vector>
#include <math.h>

struct SplineStepResult {
    v2 new_point;
    f32 unused_dt = 0;
};

struct SplineFinishedEvent {
    f32 unused_dt = 0.f;
};

struct Spline {
    // points to interpolate through
    std::vector<v2> ps;

    // derivatives at each point, determined via Catmull-Rom method
    std::vector<v2> ms;

    // ts[i != 0] specifies the time it takes to get to each point from the start position
    // and it is assumed that ts[0] == 0
    std::vector<f32> ts;

    // which segment of interpolation is currently underway
    u32 current_bin = 0;

    // time elapsed within the current interpolation segment
    f32 current_bin_duration = 0.f;

    SplineStepResult step(f32 dt, bool looping);
    inline void reset(void) {
        current_bin = 0;
        current_bin_duration = 0.f;
    }
};

// https://en.wikipedia.org/wiki/Cubic_Hermite_spline#Catmull%E2%80%93Rom_spline
Spline make_spline(const std::vector<v2>& ps, const std::vector<f32>& ts);
