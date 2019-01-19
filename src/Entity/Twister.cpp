#include "Entity/Twister.hpp"

#include "Behavior/Pause.hpp"
#include "Bullet/Bullet.hpp"
#include "Globals.hpp"
#include "Random.hpp"
#include "Spline.hpp"
#include "SpriteCache.hpp"

#include "Component/Common.hpp"
#include "Component/OffscreenBehavior.hpp"
#include "Component/BulletStream.hpp"

#include <string>

namespace {

inline v2 random_position(void) {
    return {uniform_random(-75, 75), uniform_random(-75, 75)};
}

}

namespace Twister {

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


void StateMachineSystem::run(float dt) {
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
            CM->book<EulerRotation>(uuid).vel = 1.3;
            auto& bstream = CM->book<BulletStream>(uuid, 0.05);
            bstream.add_bullet({ ENEMY_BULLET, {5,0}, {50, 0} });
            bstream.add_bullet({ ENEMY_BULLET, {-5,0}, {-50, 0} });
            bstream.add_bullet({ ENEMY_BULLET, {0,5}, {0, 50} });
            bstream.add_bullet({ ENEMY_BULLET, {0,-5}, {0, -50} });
            bstream.cycles_left = 5;

            bstream.next_state_id = PauseAfterFiring;
            break;
        }

        case PauseAfterFiring: {
            CM->remove<EulerRotation>(uuid);
            auto& pause = CM->book<PauseBehavior>(uuid);
            pause.time_left = 1.0;
            pause.next_state_id = Relocating;
            break;
        }

        default: { break; }
        }
    }
}

}
