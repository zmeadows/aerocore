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

        const v2 stabber_orientation = orientation_of(stabber);
        const v2 stabber_to_player = (player.pos - stabber.pos).normalized();
        const float distance_to_player = (player.pos - stabber.pos).magnitude();

        switch (stabber_data.state) {

            case StabberData::State::Stabbing: {
                // continue flying until past goal, at which point find new goal point and switch state to relocating
                const bool stab_missed = stabber_orientation.dot(stabber_to_player) < 0.f;
                if (stab_missed) {
                    stabber.acc = stabber.vel.normalized();
                    stabber.acc.scale(-30.f);
                    stabber_data.state = StabberData::State::Stopping;
                }
                break;
            }

            case StabberData::State::Stopping: {
                if (stabber.acc.dot(stabber.vel) >= 0)
                    set_stabber_to_relocating(stabber, stabber_data);
                break;
            }

            case StabberData::State::Relocating: {
                if (stabber.vel.magnitude() >= stabber_data.speed)
                    stabber.acc = {0.f, 0.f};

                if (distance(stabber.pos, stabber_data.idle_point) < 10.f) {
                    stabber.vel = {0.f, 0.f};
                    stabber.acc = {0.f, 0.f};

                    if (stabber_to_player.y * stabber_orientation.x
                        > stabber_to_player.x * stabber_orientation.y)
                        stabber.angvel = 1.5f;
                    else stabber.angvel = -1.5f;

                    stabber_data.state = StabberData::State::Aiming;
                }

                break;
            }
            case StabberData::State::Aiming: {
                if (stabber_to_player.dot(stabber_orientation) > 0.99) {
                    stabber.angvel = 0;
                    stabber.angle = arctan(stabber_to_player.x, stabber_to_player.y);
                    stabber.vel = (player.pos - stabber.pos).normalized();
                    stabber.vel.scale(stabber_data.speed);
                    stabber_data.state = StabberData::State::Stabbing;
                }

                break;
            }
        }
    }
}
