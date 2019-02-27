#include "Game.hpp"

#include "GraphicsContext.hpp"
#include "InputManager.hpp"
#include "AudioContext.hpp"

#include "Entity/Player.hpp"
#include "Entity/Twister.hpp"

#include "Bullet/Bullet.hpp"

#include "Component/Common.hpp"
#include "Component/CollisionData.hpp"
#include "Component/OffscreenBehavior.hpp"
#include "Component/BulletStream.hpp"

#include "QuadTree.hpp"
#include "QuadTreeDraw.hpp"
#include "Random.hpp"
#include "Spline.hpp"

#include <SDL.h>

Game::Game(void)
    : QT(3)
    , quadtree_update_sys(&QT)
    , bullet_stream_sys(&AC, &SC)
    , draw_sys(&GC)
    , draw_collision_geometry_sys(&GC)
{
    CM.registerComponent<Entity>(250);
    CM.registerComponent<EulerTranslation>(250);
    CM.registerComponent<EulerRotation>(250);
    CM.registerComponent<OffscreenBehavior>(250);
    CM.registerComponent<DestructTag>(50);
    CM.registerComponent<BulletTag>(250);
    CM.registerComponent<FriendlyTag>(100);
    CM.registerComponent<CollisionData>(250);
    CM.registerComponent<PositionUpdate>(250);
    CM.registerComponent<RotationUpdate>(250);
    CM.registerComponent<Sprite>(250);
    CM.registerComponent<StateTransition>(100);
    CM.registerComponent<Twister::Tag>(50);
    CM.registerComponent<TranslationSpline>(50);
    CM.registerComponent<PauseBehavior>(50);
    CM.registerComponent<BulletStream>(50);
    CM.registerComponent<DamageEvent>(50);
    CM.registerComponent<Health>(50);
    CM.registerComponent<CollideDamage>(250);
    CM.registerComponent<Invincibility>(50);

    CM.registerSystem(euler_translation_sys);
    CM.registerSystem(euler_rotation_sys);
    CM.registerSystem(translation_spline_sys);
    CM.registerSystem(offscreen_behavior_sys);
    CM.registerSystem(position_update_sys);
    CM.registerSystem(rotation_update_sys);
    CM.registerSystem(pause_sys);
    CM.registerSystem(twister_state_sys);
    CM.registerSystem(state_transition_cleanup_sys);
    CM.registerSystem(bullet_stream_sys);
    CM.registerSystem(invincibility_sys);
    CM.registerSystem(quadtree_update_sys);
    CM.registerSystem(collision_sys);
    CM.registerSystem(damage_sys);
    CM.registerSystem(destruct_sys);
    CM.registerSystem(draw_sys);

    generate_player(&CM, &SC);
    Twister::generate(&CM, &SC);
    last_asteroid_time = 0;
}

bool Game::tick(void) {
    m_frames_elapsed++;

    bool quitting = false;

    quitting = poll_input();

    if (SDL_GetTicks() > last_asteroid_time + 50) {
        Twister::generate(&CM, &SC);
        last_asteroid_time = SDL_GetTicks();
    }

    m_preSystemRunTicks = SDL_GetPerformanceCounter();
    const float dt = static_cast<f32>(m_preSystemRunTicks - m_postSystemRunTicks) / SDL_GetPerformanceFrequency();
    time_elapsed += dt;
    m_postSystemRunTicks = m_preSystemRunTicks;

    tick_player(&CM, &AC, &SC, &input, &state, dt);

    auto pre_run = SDL_GetPerformanceCounter();

    CM.tick(dt);

    auto post_run = SDL_GetPerformanceCounter();
    const float run_time = static_cast<float>(post_run - pre_run) / SDL_GetPerformanceFrequency();
    if (m_frames_elapsed % 50 == 0) {
        std::cout << "frame compute time: " << run_time << " :: " << CM.alive_count() << " entities on screen" << std::endl;
    }

    GPU_Flip(GC.renderer);

    quitting |= !CM.has<Entity>(playerUUID());

    return quitting;
}

bool Game::poll_input(void) {
    SDL_Event e;

    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT)
            return true;

        if (e.type == SDL_CONTROLLERAXISMOTION) {
            JoystickMotion motion;
            motion.value = static_cast<f32>(e.caxis.value) / 32768.f;

			switch (e.caxis.axis) {
                case SDL_CONTROLLER_AXIS_LEFTX:
                    motion.axis = X_AXIS;
                    motion.which = LEFT_JOYSTICK;
                    process_input(&input, motion);
                    break;
                case SDL_CONTROLLER_AXIS_LEFTY:
                    motion.axis = Y_AXIS;
                    motion.which = LEFT_JOYSTICK;
                    motion.value *= -1.f;
                    process_input(&input, motion);
                    break;
                case SDL_CONTROLLER_AXIS_RIGHTX:
                    motion.axis = X_AXIS;
                    motion.which = RIGHT_JOYSTICK;
                    process_input(&input, motion);
                    break;
                case SDL_CONTROLLER_AXIS_RIGHTY:
                    motion.axis = Y_AXIS;
                    motion.which = RIGHT_JOYSTICK;
                    motion.value *= -1.f;
                    process_input(&input, motion);
                    break;
                default:
                    break;
			}
        }
    }

    return false;
}

Game::~Game(void) {
    CM.unRegisterComponent<Entity>();
    CM.unRegisterComponent<EulerTranslation>();
    CM.unRegisterComponent<EulerRotation>();
    CM.unRegisterComponent<OffscreenBehavior>();
    CM.unRegisterComponent<DestructTag>();
    CM.unRegisterComponent<BulletTag>();
    CM.unRegisterComponent<FriendlyTag>();
    CM.unRegisterComponent<CollisionData>();
    CM.unRegisterComponent<PositionUpdate>();
    CM.unRegisterComponent<RotationUpdate>();
    CM.unRegisterComponent<Sprite>();
    CM.unRegisterComponent<StateTransition>();
    CM.unRegisterComponent<Twister::Tag>();
    CM.unRegisterComponent<TranslationSpline>();
    CM.unRegisterComponent<PauseBehavior>();
    CM.unRegisterComponent<BulletStream>();
    CM.unRegisterComponent<DamageEvent>();
    CM.unRegisterComponent<Health>();
    CM.unRegisterComponent<CollideDamage>();
    CM.unRegisterComponent<Invincibility>();
}
