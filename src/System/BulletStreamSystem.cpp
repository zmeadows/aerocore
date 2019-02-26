#include "System/BulletStreamSystem.hpp"

#include "Bullet/Bullet.hpp"
#include "Engine/ComponentManager.hpp"
#include "AudioContext.hpp"

void BulletStreamSystem::run(ComponentManager* CM, f32 dt) {
    for (const UUID uuid : this->followed) {
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

                generate_bullet(CM, AC, SC, spec.type, bullet_position, bullet_velocity, friendly);
            }

            if (bstream.cycles_left > 0)
                bstream.cycles_left--;

            if (bstream.cycles_left == 0) {
                auto& transition = CM->book<StateTransition>(uuid);
                transition.next_state_id = bstream.next_state_id;
                transition.extra_time = -1.f * bstream.current_countdown;
                this->finished.append(uuid);
            } else {
                bstream.current_countdown += bstream.delay_per_bullet;
            }
        }
    }

    for (const UUID uuid : this->finished) {
        CM->remove<BulletStream>(uuid);
    }

    this->finished.clear();
}
