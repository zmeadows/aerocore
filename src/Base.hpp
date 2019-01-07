#pragma once

#include <limits>
#include <iostream>
#include <vector>

#include "Vector2D.hpp"
#include "Typedef.hpp"
#include "Const.hpp"

class UUID;

float uniform_rand(float min, float max);

// struct GlobalVertexBuffer {
//     v2 data[1<<5];
//     u32 count = 0;
//
//     v2 operator[](u32 idx) const { return data[idx]; }
//     v2& operator[](u32 idx) { return data[idx]; }
//
//     v2* begin() { return &data[0]; }
//     v2* end() { return &data[count]; }
//     const v2* begin() const { return &data[0]; }
//     const v2* end() const { return &data[count]; }
// };

struct LocalVertexBuffer {
    v2 data[1<<5];
    u32 count = 0;

    v2 operator[](u32 idx) const { return data[idx]; }
    v2& operator[](u32 idx) { return data[idx]; }

    v2* begin() { return &data[0]; }
    v2* end() { return &data[count]; }
    const v2* begin() const { return &data[0]; }
    const v2* end() const { return &data[count]; }
};

//TODO: use std::variant
struct OffscreenBehavior {
    enum class Type {
        DiesInstantly,
        SinglePassAllowed,
        ValidRange,
        Wraps
    } type = Type::DiesInstantly;

    union {
        struct {
            float minX = -250.f;
            float maxX = 250.f;
            float minY = -250.f;
            float maxY = 250.f;
        } ValidRange;

        struct {
            bool already_found_onscreen = false;
        } SinglePassAllowed;
    };

    OffscreenBehavior(void) {}
};

const UUID playerUUID(void);

//@TODO: move to Sprite file
struct Extent {
    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::lowest();
};

void dump(const Extent& extent);

Extent extent_of(const std::vector<v2>& global_vertices);
Extent extent_of(const LocalVertexBuffer& local_vertices, v2 position, f32 angle);

inline float signum(float x) {
    return static_cast<float>(x > 0) - static_cast<float>(x < 0);
}

struct DeathTimer {
    float value = 0.0;
};

struct ShotDelay {
    float delay = 0.0;
    float lastShotTime = 0.0;
};

Extent clip_to_screen(const Extent& ext);

inline float rotate(float& old_angle, float delta) {
    const float new_angle = fmod(old_angle + delta, TWOPI);
    return (new_angle < 0) ? new_angle + TWOPI : new_angle;
}

inline float arctan(float x, float y) {
    return fmod(atan2(y,x) + TWOPI, TWOPI);
}

//@CLARITY: give these distinct names
bool is_offscreen(const Extent& ext);

std::vector<v2> compute_global_vertices( const LocalVertexBuffer& lvb
                                       , const v2 position_offset
                                       , const float rotation_angle);

