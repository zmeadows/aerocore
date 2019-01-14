#pragma once

#include "Globals.hpp"
#include "Entity.hpp"
#include "State.hpp"

struct v2;
class UUID;

enum BulletType {
    PLAYER_BULLET,
    ENEMY_BULLET
};

UUID generate_bullet(BulletType bullet_type,
                     const v2 new_bullet_position,
                     const v2 new_bullet_velocity,
                     bool friendly);

struct BulletSpec {
    BulletType type = PLAYER_BULLET;
    v2 shooter_offset;
    v2 velocity;
};

namespace {
    const u32 MAX_BULLETSTREAM_SIZE = 10;
}

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

    void add_bullet(BulletSpec bullet);
};

inline void BulletStream::add_bullet(BulletSpec bullet) {
    assert(num_specs < MAX_BULLETSTREAM_SIZE);
    specs[num_specs] = bullet;
    num_specs++;
}

class BulletStreamSystem final : public System {
public:
    BulletStreamSystem(void) : System("BulletStream") {
        get_manager()->subscribe<Entity, BulletStream>(this);
    }

    void run(float dt) final {
        auto CM = get_manager();

        std::vector<UUID> finished;

        for (const UUID uuid : m_followed) {
            auto& entity = CM->get<Entity>(uuid);
            auto& bstream = CM->get<BulletStream>(uuid);

            bstream.current_countdown -= dt;

            if (bstream.current_countdown <= 0.f) {
                for (u32 idx = 0; idx < bstream.num_specs; idx++) {
                    const BulletSpec& spec = bstream.specs[idx];

                    const bool friendly = CM->has<FriendlyTag>(uuid);
                    const f32 shooter_angle = entity.angle;
                    const v2 shooter_position = entity.pos;

                    const v2 bullet_position = shooter_position + spec.shooter_offset.rotated(shooter_angle);
                    const v2 bullet_velocity = spec.velocity.rotated(shooter_angle);

                    generate_bullet(spec.type, bullet_position, bullet_velocity, friendly);
                }


                if (bstream.cycles_left > 0) bstream.cycles_left--;

                if (bstream.cycles_left == 0) {
                    auto& transition = CM->book<StateTransition>(uuid);
                    transition.next_state_id = bstream.next_state_id;
                    transition.extra_time = -1.0 * bstream.current_countdown;
                    finished.push_back(uuid);
                } else {
                    bstream.current_countdown += bstream.delay_per_bullet;
                }
            }
        }


        for (const UUID uuid : finished) {
            CM->remove<BulletStream>(uuid);
        }
    }
};


