#include "System/StabberSystem.hpp"
#include "Globals.hpp"

#include "Entity.hpp"

#include <math.h>

void StabberSystem::run(float dt) {
    auto CM = get_manager();

    const Entity& player = CM->get<Entity>(playerUUID());

    for (const UUID uuid : m_followed) {
        auto& stabber = CM->get<Entity>(uuid);
        auto& stabber_data = CM->get<StabberData>(uuid);

        switch (stabber_data.state) {

            case StabberData::State::Stabbing: {
                // continue flying until past goal, at which point find new goal point and switch state to relocating
                const bool stab_missed = stabber.vel.normalized().dot( (player.pos - stabber.pos).normalized() ) < 0.25f;
                if (stab_missed)
                    set_stabber_to_relocating(stabber, stabber_data);
                break;
            }

            case StabberData::State::Relocating: {
                if (stabber.vel.magnitude() >= 20.f)
                    stabber.acc = {0.f, 0.f};

                if (distance(stabber.pos, stabber_data.idle_point) < 10.f) {
                    stabber.vel = {0.f, 0.f};
                    stabber.acc = {0.f, 0.f};

                    const v2 ov = { -std::sin(stabber.angle), std::cos(stabber.angle) };
                    const v2 stp = (player.pos - stabber.pos).normalized();

                    if (stp.y * ov.x > stp.x * ov.y)
                        stabber.angvel = 1.5f;
                    else
                        stabber.angvel = -1.5f;

                    stabber_data.state = StabberData::State::Aiming;
                }

                break;
            }
            case StabberData::State::Aiming: {
                const v2 stabber_to_player = (player.pos - stabber.pos).normalized();
                const v2 orientation_vector = { -std::sin(stabber.angle), std::cos(stabber.angle) };

                if (stabber_to_player.dot(orientation_vector) > 0.99) {
                    stabber.angvel = 0;
                    // stabber.angle = angle_to_player;
                    stabber.vel = (player.pos - stabber.pos).normalized();
                    stabber.vel.scale(stabber_data.speed);
                    stabber_data.state = StabberData::State::Stabbing;
                }

                break;
            }
        }
    }
}
