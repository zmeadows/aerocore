#pragma once

#include "Entity.hpp"
#include "Globals.hpp"
#include "../Generator/Bullet.hpp"
#include <string>

namespace Twister {

enum class State {
    ComingOnScreen,
    Firing
};

struct Data {
    State state = State::ComingOnScreen;
    f32 shot_delay = 0;
};

void generate(v2 position) {
    UUID uuid;
    generate_enemy_skeleton(uuid, position, "sprites/twister/twister.png", "sprites/twister/path");

    auto CM = get_manager();

    auto& kin = CM->book<EulerTranslation>(uuid);
    kin.vel = { 0.f, -50.f };

    auto& osb = CM->get<OffscreenBehavior>(uuid);
    osb.type = OffscreenBehavior::Type::SinglePassAllowed;
    osb.SinglePassAllowed.already_found_onscreen = false;

    CM->book<Twister::Data>(uuid);
}

class System final : public ::System {
public:
    System(void) : ::System("Twister") {
        get_manager()->subscribe<Entity, OffscreenBehavior, Twister::Data>(this);
    }

    void run(float dt) final {
        auto CM = get_manager();

        for (const UUID uuid : m_followed) {
            auto& data = CM->get<Twister::Data>(uuid);

            switch (data.state) {
                case State::ComingOnScreen: {
                    const auto& osb = CM->get<OffscreenBehavior>(uuid);
                    if (osb.SinglePassAllowed.already_found_onscreen) {
                        std::cout << "found twister on screen" << std::endl;
                        data.state = State::Firing;
                        data.shot_delay = 1.0;
                    }
                    break;
                }

                case State::Firing: {
                    data.shot_delay -= dt;
                    if (data.shot_delay < 0) {
                        const auto& entity = CM->get<Entity>(uuid);

                        const v2 bullet_offsets[4] = {
                            { 0.0, 0.7 * (entity.extent.minY - entity.extent.maxY) },
                            { 0.0, - 0.7 * (entity.extent.minY - entity.extent.maxY) },
                            { 0.7 * (entity.extent.minY - entity.extent.maxY) , 0.0 },
                            { - 0.7 * (entity.extent.minY - entity.extent.maxY), 0.0 }
                        };

                        const v2 bullet_velocities[4] = {
                            { 0, -100 },
                            { 0, 100 },
                            { -100, 0 },
                            { 100, 0 }
                        };

                        for (auto i = 0; i < 4; i++) {
                            generate_bullet_enemy(entity.pos + bullet_offsets[i], bullet_velocities[i], 0);
                        }

                        data.shot_delay = data.shot_delay + 1.0;
                    }
                    break;
                }
            }
        }
    }
};

}
