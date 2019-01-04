#pragma once

#include "Vector2D.hpp"

#include <vector>
#include <math.h>

// https://en.wikipedia.org/wiki/Cubic_Hermite_spline#Catmull%E2%80%93Rom_spline

namespace {
    f32 h00(f32 t) { return 2 * std::pow(t, 3) - 3 * std::pow(t,2) + 1; }
    f32 h10(f32 t) { return std::pow(t, 3) - 2 * std::pow(t,2) + t; }
    f32 h01(f32 t) { return -2 * std::pow(t, 3) + 3 * std::pow(t,2); }
    f32 h11(f32 t) { return std::pow(t, 3) - std::pow(t,2); }
}

struct SplineStepResult {
    v2 new_point;
    f32 unused_dt = 0;
};

struct Spline {
    std::vector<v2> ps;
    std::vector<v2> ms;
    std::vector<f32> ts;

    u32 bin = 0;
    f32 t_bin = 0;

    SplineStepResult step(const f32 dt) {
        f32 bin_time = ts[bin+1] - ts[bin];
        t_bin += dt;

        if (t_bin > bin_time) {
            if (bin < ps.size() - 2) {
                bin++;
                t_bin -= bin_time;
                bin_time = ts[bin+1] - ts[bin];
            } else {
                SplineStepResult finished;
                finished.new_point = ps.back();
                finished.unused_dt = t_bin - bin_time;
                return finished;
            }
        }

        const f32 t = t_bin / bin_time;

        SplineStepResult step;

        step.new_point = h00(t) * ps[bin] + h10(t) * bin_time * ms[bin] + h01(t) * ps[bin+1] + h11(t) * bin_time * ms[bin+1];

        return step;
    }
};

Spline make_spline(const std::vector<v2>& ps, const std::vector<f32>& ts) {
    assert(ts.size() == ps.size());

    const auto N = ps.size();

    assert(N >= 3);

    std::vector<v2> ms;
    ms.reserve(N);

    ms.push_back( (ps[1] - ps[0]) * (1.0 / (ts[1] - ts[0])) );

    for (u32 k = 1; k < N - 1; k++) {
        v2 slope = (ps[k+1] - ps[k-1]) * (1.0 / (ts[k+1] - ts[k-1]));
        ms.push_back(slope);
    }

    ms.push_back( (ps[N-1] - ps[N-2]) * (1.0 / (ts[N-1] - ts[N-2])) );

    assert(ms.size() == N);

    Spline s;

    s.ps = ps;
    s.ts = ts;
    s.ms = ms;

    return s;
}
