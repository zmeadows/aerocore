#include "Spline.hpp"

#include <assert.h>

namespace {
    inline f32 h00(f32 t) { return 2 * std::pow(t, 3) - 3 * std::pow(t,2) + 1; }
    inline f32 h10(f32 t) { return std::pow(t, 3) - 2 * std::pow(t,2) + t; }
    inline f32 h01(f32 t) { return -2 * std::pow(t, 3) + 3 * std::pow(t,2); }
    inline f32 h11(f32 t) { return std::pow(t, 3) - std::pow(t,2); }
}

SplineStepResult Spline::step(f32 dt, bool looping) {
    f32 bin_time = ts[current_bin+1] - ts[current_bin]; // total duration of current interpolation segment

    current_bin_duration += dt;

    if (current_bin_duration > bin_time) { // transition between interpolation segments is occuring

        //TODO: account for cases where dt is big enough to jump a whole bin
        //      ... even though this should absolutely never happen

        // case 1: normal transition, i.e. not transitioning out of last interpolation segment
        if (current_bin < ps.size() - 2) {
            current_bin++;
            current_bin_duration -= bin_time;
            bin_time = ts[current_bin+1] - ts[current_bin];

            // case 2: finished spline, but looping
        } else if (looping) {
            current_bin = 0;
            current_bin_duration -= bin_time;
            bin_time = ts[1] - ts[0];

            // case 3: finished spline, not looping (so return unused time increment)
        } else {
            SplineStepResult finished;
            finished.new_point = ps.back();
            finished.unused_dt = current_bin_duration - bin_time;
            return finished;
        }
    }

    const f32 t = current_bin_duration / bin_time;

    SplineStepResult step;

    const v2 y0 = h00(t) * ps[current_bin];
    const v2 y1 = h10(t) * bin_time * ms[current_bin];
    const v2 y2 = h01(t) * ps[current_bin+1];
    const v2 y3 = h11(t) * bin_time * ms[current_bin+1];

    step.new_point = y0 + y1 + y2 + y3;

    return step;
}

// https://en.wikipedia.org/wiki/Cubic_Hermite_spline#Catmull%E2%80%93Rom_spline
// TODO: add tension parameter 'c'
Spline make_spline(const std::vector<v2>& ps, const std::vector<f32>& ts) {
    assert(ts.size() == ps.size());
    assert(ts.size() >= 3);

    const auto N = ps.size();

    const bool looping = ps[0] == ps[N-1];

    std::vector<v2> ms;
    ms.reserve(N);

    if (looping) {
        float t1 = ts[1] - ts[0];
        float t2 = ts[N-1] - ts[N-2];
        ms.push_back( (ps[1] - ps.back()) / (t1 + t2) );
    } else {
        ms.push_back( (ps[1] - ps[0]) / (ts[1] - ts[0]) );
    }

    for (u32 k = 1; k < N - 1; k++) {
        v2 slope = (ps[k+1] - ps[k-1]) / (ts[k+1] - ts[k-1]);
        ms.push_back(slope);
    }

    if (looping) {
        ms.push_back( ms[0] );
    } else {
        ms.push_back( (ps[N-1] - ps[N-2]) / (ts[N-1] - ts[N-2]) );
    }

    assert(ms.size() == N);

    Spline s;

    s.ps = ps;
    s.ts = ts;
    s.ms = ms;

    return s;
}
