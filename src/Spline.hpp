#pragma once

#include "Vector2D.hpp"
#include "Typedef.hpp"
#include "Component/Common.hpp"
#include "Globals.hpp"
#include "Component/StateTransition.hpp"

#include <vector>
#include <math.h>
#include <optional>

namespace {
    const u32 SPLINE_BUFFER_SIZE = 10;

    inline f32 h00(f32 t) { return 2 * std::pow(t, 3) - 3 * std::pow(t,2) + 1; }
    inline f32 h10(f32 t) { return std::pow(t, 3) - 2 * std::pow(t,2) + t; }
    inline f32 h01(f32 t) { return -2 * std::pow(t, 3) + 3 * std::pow(t,2); }
    inline f32 h11(f32 t) { return std::pow(t, 3) - std::pow(t,2); }
}

template <typename T>
struct SplineStepResult {
    T new_point;
    bool finished = false;
    f32 leftover_dt = 0;
};

template <typename T>
struct Spline {
    // points to interpolate through
    T ps[SPLINE_BUFFER_SIZE];

    // derivatives at each point, determined via Catmull-Rom method
    T ms[SPLINE_BUFFER_SIZE];

    // ts[i != 0] specifies the time it takes to get to each point from the start position
    // and it is assumed that ts[0] == 0
    f32 ts[SPLINE_BUFFER_SIZE];

    // number of points in spline
    u32 count = 0;

    // which segment of interpolation is currently underway
    u32 current_bin = 0;

    // time elapsed within the current interpolation segment
    f32 current_bin_duration = 0.f;

    bool looping = false;

    void add_point(const T& new_point, f32 new_time);

    void construct(void);

    SplineStepResult<T> step(f32 dt);
};

template <typename T>
void Spline<T>::add_point(const T& new_point, f32 new_time) {
    ps[count] = new_point;
    ts[count] = new_time;
    count++;
    assert(count <= SPLINE_BUFFER_SIZE);
}

template <typename T>
SplineStepResult<T> Spline<T>::step(f32 dt) {
    f32 bin_time = ts[current_bin+1] - ts[current_bin]; // total duration of current interpolation segment

    current_bin_duration += dt;

    if (current_bin_duration > bin_time) {
        // transition between interpolation segments is occuring

        //TODO: account for cases where dt is big enough to jump a whole bin
        //      ... even though this should absolutely never happen

        // case 1: normal transition, i.e. not transitioning out of last interpolation segment
        if (current_bin < count - 2) {
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
            SplineStepResult<T> result;
            result.finished = true;
            result.new_point = ps[count-1];
            result.leftover_dt = current_bin_duration - bin_time;
            return result;
        }

    }

    const f32 t = current_bin_duration / bin_time;

    SplineStepResult<T> step;

    const T y0 = h00(t) * ps[current_bin];
    const T y1 = h10(t) * bin_time * ms[current_bin];
    const T y2 = h01(t) * ps[current_bin+1];
    const T y3 = h11(t) * bin_time * ms[current_bin+1];

    step.new_point = y0 + y1 + y2 + y3;

    return step;
}

// https://en.wikipedia.org/wiki/Cubic_Hermite_spline#Catmull%E2%80%93Rom_spline
// TODO: add tension parameter 'c'
template <typename T>
void Spline<T>::construct(void) {
    if (looping) {
        const float t1 = ts[1] - ts[0];
        const float t2 = ts[count-1] - ts[count-2];
        ms[0] = (ps[1] - ps[count-1]) / (t1 + t2);
        ms[count - 1] = ms[0];
    } else {
        ms[0] = (ps[1] - ps[0]) / (ts[1] - ts[0]);
        ms[count - 1] = (ps[count-1] - ps[count-2]) / (ts[count-1] - ts[count-2]);
    }

    for (u32 k = 1; k < count - 1; k++)
        ms[k] = (ps[k+1] - ps[k-1]) / (ts[k+1] - ts[k-1]);
}

struct TranslationSpline : Spline<v2> {
    std::optional<s32> next_state_id;
};

// struct RotationSpline : Spline<f32> {
//     std::optional<s32> next_state_id;
// };

//TODO: move to own file
class TranslationSplineSystem final : public System {

public:
    void run(float dt) final {
        auto CM = get_manager();

		std::vector<UUID> post_run_uuid_buffer;

        for (const UUID uuid : m_followed) {
            auto& spline = CM->get<TranslationSpline>(uuid);
            auto& update = CM->get<PositionUpdate>(uuid);
            const auto& entity = CM->get<Entity>(uuid);

            SplineStepResult<v2> result = spline.step(dt);

            update.delta = result.new_point - entity.pos;

            if (result.finished) {
                post_run_uuid_buffer.push_back(uuid);
                if (spline.next_state_id) {
                    auto& transition = CM->book<StateTransition>(uuid);
                    transition.next_state_id = *spline.next_state_id;
                    transition.extra_time = result.leftover_dt;
                }
            }
        }

        for (const UUID uuid : post_run_uuid_buffer) {
            CM->remove<TranslationSpline>(uuid);
        }

    }

    TranslationSplineSystem(void) : System("TranslationSpline")
    {
        get_manager()->subscribe<Entity, TranslationSpline, PositionUpdate>(this);
    }
};

