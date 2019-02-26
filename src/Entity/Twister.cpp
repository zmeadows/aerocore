#include "Entity/Twister.hpp"

#include "Bullet/Bullet.hpp"
#include "Engine/ComponentManager.hpp"
#include "Random.hpp"
#include "Spline.hpp"
#include "SpriteCache.hpp"


namespace {

inline v2 random_position(void) {
    return {uniform_random<f32>(-75, 75), uniform_random<f32>(-75, 75)};
}

}

namespace Twister {

enum State {
    Relocating,
    PauseBeforeFiring,
    Firing,
    PauseAfterFiring
};

void generate(ComponentManager* CM, SpriteCache* SC) {
    UUID uuid;

    auto& entity = CM->book<Entity>(uuid);
    entity.pos = {uniform_random<f32>(-100, 100), 130.f};

    SC->attach_sprite_to_uuid(CM, uuid, "twister_idle");

    CM->book<Twister::Tag>(uuid);
    CM->book<Health>(uuid);

    auto& osb = CM->book<OffscreenBehavior>(uuid);
    osb.type = OffscreenBehavior::Type::SinglePassAllowed;
    osb.SinglePassAllowed.already_found_onscreen = false;

    CM->book<StateTransition>(uuid).next_state_id = Relocating;
    CM->book<CollideDamage>(uuid);
}

void StateMachineSystem::run(ComponentManager* CM, f32 dt) {
    for (const UUID uuid : this->followed) {
        const auto& transition = CM->get<StateTransition>(uuid);

        switch (transition.next_state_id) {
            case Relocating: {
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
                CM->book<EulerRotation>(uuid).vel = 3.f;
                auto& bstream = CM->book<BulletStream>(uuid, 0.1);
                bstream.add_bullet({ ENEMY_BULLET, {5,0}, {50, 0} });
                bstream.add_bullet({ ENEMY_BULLET, {-5,0}, {-50, 0} });
                bstream.add_bullet({ ENEMY_BULLET, {0,5}, {0, 50} });
                bstream.add_bullet({ ENEMY_BULLET, {0,-5}, {0, -50} });
                bstream.cycles_left = 10;
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

            default: { assert(false && "Invalid state id"); }
        }
    }
}

}
