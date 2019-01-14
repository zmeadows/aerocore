#pragma once

#include "Behavior/Pause.hpp"
#include "Bullet/Bullet.hpp"
#include "Entity.hpp"
#include "Globals.hpp"
#include "Random.hpp"
#include "Spline.hpp"
#include "SpriteCache.hpp"

#include <string>

namespace {

inline v2 random_position(void) {
    return {uniform_random(-75, 75), uniform_random(-75, 75)};
}

}

namespace Twister {

struct Tag {};

enum State {
    Relocating,
    PauseBeforeFiring,
    Firing,
    PauseAfterFiring
};

void generate(void) {
    UUID uuid;

    auto CM = get_manager();

    auto& entity = CM->book<Entity>(uuid);
    entity.pos = {uniform_random(-100, 100), 130.f};

    get_sprite_cache()->attach_sprite_to_uuid(uuid, "twister_idle");

    CM->book<Twister::Tag>(uuid);

    auto& osb = CM->book<OffscreenBehavior>(uuid);
    osb.type = OffscreenBehavior::Type::SinglePassAllowed;
    osb.SinglePassAllowed.already_found_onscreen = false;

    CM->book<StateTransition>(uuid).next_state_id = Relocating;
}


class StateMachineSystem final : public System {
public:
    StateMachineSystem(void) : System("Twister::StateMachine") {
        get_manager()->subscribe<Entity, StateTransition, Twister::Tag>(this);
    }

    void run(float dt) final {
        auto CM = get_manager();

        for (const UUID uuid : m_followed) {
            const auto& transition = CM->get<StateTransition>(uuid);

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

            case PauseBeforeFiring: {
                auto& pause = CM->book<PauseBehavior>(uuid);
                pause.time_left = 1.0;
                pause.next_state_id = Firing;
                break;
            }

            case Firing: {
                auto& bstream = CM->book<BulletStream>(uuid, 0.1);
                bstream.add_bullet({ ENEMY_BULLET, {5,0}, {110, 0} });
                bstream.add_bullet({ ENEMY_BULLET, {-5,0}, {-110, 0} });
                bstream.add_bullet({ ENEMY_BULLET, {0,5}, {0, 110} });
                bstream.add_bullet({ ENEMY_BULLET, {0,-5}, {0, -110} });
                bstream.cycles_left = 4;

                bstream.next_state_id = PauseAfterFiring;
                break;
            }

            case PauseAfterFiring: {
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

}
