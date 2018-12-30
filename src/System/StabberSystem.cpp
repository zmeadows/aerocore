#include "System/StabberSystem.hpp"
#include "Globals.hpp"
#include "Entity.hpp"

#include <math.h>

void StabberSystem::run(float /*dt*/) {
    auto CM = get_manager();

    const Entity& player = CM->get<Entity>(playerUUID());

    for (const UUID uuid : m_followed) {
        auto& stabber = CM->get<Entity>(uuid);
        auto& kin = CM->get<EulerTranslation>(uuid);
        auto& rot = CM->get<EulerRotation>(uuid);
        auto& stabber_data = CM->get<Stabber>(uuid);

        const v2 stabber_orientation = orientation_of(stabber);
        const v2 stabber_to_player = (player.pos - stabber.pos).normalized();

        switch (stabber_data.state) {

            case Stabber::State::Stabbing: {
                // continue flying until past goal, at which point find new goal point and switch state to relocating
                const bool stab_missed = stabber_orientation.dot(stabber_to_player) < 0.f;
                if (stab_missed) {
                    kin.acc = { 0.f, 0.f };
                    kin.drag = 75.f;
                    stabber_data.state = Stabber::State::Stopping;
                }
                break;
            }

            case Stabber::State::Stopping: {
                if (kin.vel.magnitude() < 1.f) {
                    kin.drag = 0.f;
                    stabber_data.idle_point = { uniform_rand(-80.f, 80.f), uniform_rand(-80.f, 80.f) };
                    kin.acc = 6.0 * (stabber_data.idle_point - stabber.pos).normalized();
                    stabber_data.state = Stabber::State::Relocating;
                }
                break;
            }

            case Stabber::State::Relocating: {
                if (kin.vel.magnitude() >= stabber_data.speed)
                    kin.acc = {0.f, 0.f};

                if (distance(stabber.pos, stabber_data.idle_point) < 10.f) {
                    kin.vel = {0.f, 0.f};
                    kin.acc = {0.f, 0.f};

                    if (stabber_to_player.y * stabber_orientation.x
                        > stabber_to_player.x * stabber_orientation.y)
                        rot.vel = 1.5f;
                    else rot.vel = -1.5f;

                    stabber_data.state = Stabber::State::Aiming;
                }

                break;
            }

            case Stabber::State::Aiming: {
                if (stabber_to_player.dot(stabber_orientation) > 0.99f) {
                    rot.vel = 0;
                    stabber.angle = arctan(stabber_to_player.x, stabber_to_player.y);
                    kin.vel = (player.pos - stabber.pos).normalized();
                    kin.vel.scale(stabber_data.speed);
                    stabber_data.state = Stabber::State::Stabbing;
                }

                break;
            }
        }
    }
}
