#include "System/BulletStreamSystem.hpp"

#include "Globals.hpp"
#include "Bullet/Bullet.hpp"
#include "Component/Common.hpp"
#include "Component/BulletStream.hpp"
#include "Component/StateTransition.hpp"

BulletStreamSystem::BulletStreamSystem(void) : System("BulletStream") {
    get_manager()->subscribe<Entity, BulletStream>(this);
}

void BulletStreamSystem::run(float dt) {
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
                transition.extra_time = -1.f * bstream.current_countdown;
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

