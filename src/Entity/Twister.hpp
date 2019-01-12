#pragma once

#include "../Generator/Bullet.hpp"
#include "Behavior/Pause.hpp"
#include "Entity.hpp"
#include "Globals.hpp"
#include "Random.hpp"
#include "Spline.hpp"

#include <string>

namespace Twister {

struct Tag {};

enum State { Relocating = 0, Firing = 1 };

void generate(void) {
    UUID uuid;

    v2 init_position = {uniform_random(-100, 100), 130.f};
    generate_enemy_skeleton(uuid, init_position, "sprites/twister/twister.png", "sprites/twister/path");

    auto CM = get_manager();

    CM->book<Twister::Tag>(uuid);

    auto& osb = CM->get<OffscreenBehavior>(uuid);
    osb.type = OffscreenBehavior::Type::SinglePassAllowed;
    osb.SinglePassAllowed.already_found_onscreen = false;

    CM->book<StateTransition>(uuid).next_state_id = Relocating;
}

inline v2 random_position(void) {
    return {uniform_random(-75, 75), uniform_random(-75, 75)};
}

class StateMachineSystem final : public System {
public:
    StateMachineSystem(void) : ::System("Twister::StateMachineSystem") {
        get_manager()->subscribe<Entity, StateTransition, Twister::Tag>(this);
    }

    void run(float dt) final {
        auto CM = get_manager();

        for (const UUID uuid : m_followed) {
            auto& transition = CM->get<StateTransition>(uuid);

            switch (transition.next_state_id) {
            case Relocating: {
                assert(!CM->has<TranslationSpline>(uuid));
                auto& spline = CM->book<TranslationSpline>(uuid);
                spline.add_point(CM->get<Entity>(uuid).pos, 0.0);
                spline.add_point(random_position(), 1.0);
                spline.add_point(random_position(), 2.5);
                spline.construct();
                spline.next_state_id = Firing;

                break;
            }

            case Firing: {
                auto& pause = CM->book<PauseBehavior>(uuid);
                pause.time_left = 1.0;
                pause.next_state_id = Relocating;
                break;
            }

            default: { break; }
            }
        }
    }
};

} // namespace Twister
