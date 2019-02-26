#pragma once

#include "unstd/types.hpp"
#include "Bullet/Bullet.hpp"
#include "Vector2D.hpp"

struct BulletSpec {
    BulletType type = PLAYER_BULLET;
    v2 shooter_offset;
    v2 velocity;
};

namespace {
    const u32 MAX_BULLETSTREAM_SIZE = 10;
}

//TODO: remove constructor here?
// just make set_delay(BulletStream&) function
struct BulletStream {
    BulletSpec specs[MAX_BULLETSTREAM_SIZE];
    u32 num_specs;

    f32 delay_per_bullet;
    f32 current_countdown;

    s32 cycles_left;

    s32 next_state_id;

    BulletStream(f32 delay)
        : num_specs(0),
        delay_per_bullet(delay),
        current_countdown(delay),
        cycles_left(-1),
        next_state_id(-1) {}

    inline void add_bullet(BulletSpec bullet) {
        assert(num_specs < MAX_BULLETSTREAM_SIZE);
        specs[num_specs] = bullet;
        num_specs++;
    }
};

