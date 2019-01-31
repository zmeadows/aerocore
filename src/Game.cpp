#include "Game.hpp"

#include "Globals.hpp"
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

#include "QuadTreeDraw.hpp"
#include "Random.hpp"
#include "Spline.hpp"

#include <SDL.h>

Game::Game(void)
{
    auto CM = get_manager();

    CM->registerComponent<Entity>(250);
    CM->registerComponent<EulerTranslation>(250);
    CM->registerComponent<EulerRotation>(250);
    CM->registerComponent<OffscreenBehavior>(250);
    CM->registerComponent<DestructTag>(250);
    CM->registerComponent<BulletTag>(250);
    CM->registerComponent<FriendlyTag>(250);
    CM->registerComponent<CollisionData>(250);
    CM->registerComponent<PositionUpdate>(250);
    CM->registerComponent<RotationUpdate>(250);
    CM->registerComponent<Sprite>(250);
    CM->registerComponent<StateTransition>(250);
    CM->registerComponent<Twister::Tag>(250);
    CM->registerComponent<TranslationSpline>(250);
    CM->registerComponent<PauseBehavior>(250);
    CM->registerComponent<BulletStream>(250);
    CM->registerComponent<DamageEvent>(250);
    CM->registerComponent<Health>(250);
    CM->registerComponent<CollideDamage>(250);
    CM->registerComponent<Invincibility>(250);

    ACTIVATE_SYSTEM(euler_translation_sys);
    ACTIVATE_SYSTEM(euler_rotation_sys);
    ACTIVATE_SYSTEM(translation_spline_sys);
    ACTIVATE_SYSTEM(offscreen_behavior_sys);
    ACTIVATE_SYSTEM(position_update_sys);
    ACTIVATE_SYSTEM(rotation_update_sys);
    ACTIVATE_SYSTEM(pause_sys);
    ACTIVATE_SYSTEM(twister_state_sys);
    ACTIVATE_SYSTEM(state_transition_cleanup_sys);
    ACTIVATE_SYSTEM(bullet_stream_sys);
    ACTIVATE_SYSTEM(invincibility_sys);
    ACTIVATE_SYSTEM(quadtree_update_sys);
    ACTIVATE_SYSTEM(collision_sys);
    ACTIVATE_SYSTEM(damage_sys);
    ACTIVATE_SYSTEM(destruct_sys);
    ACTIVATE_SYSTEM(draw_sys);
    // ACTIVATE_SYSTEM(draw_collision_geometry_sys);

    generate_player();
    Twister::generate();
    last_asteroid_time = 0;
}

bool Game::tick(void) {
    m_frames_elapsed++;

    bool quitting = false;

    quitting = poll_input();

    if (SDL_GetTicks() > last_asteroid_time + 10000) {
        Twister::generate();
        last_asteroid_time = SDL_GetTicks();
    }

    m_preSystemRunTicks = SDL_GetPerformanceCounter();
    const float dt = static_cast<float>(m_preSystemRunTicks - m_postSystemRunTicks) / SDL_GetPerformanceFrequency();
    time_elapsed += dt;
    m_postSystemRunTicks = m_preSystemRunTicks;

    //TODO: why pointers here?
    tick_player(&input, &state, dt);

    auto pre_run = SDL_GetPerformanceCounter();

    run(euler_translation_sys, dt);
    run(euler_rotation_sys, dt);
    run(translation_spline_sys, dt);
    run(offscreen_behavior_sys);
    run(position_update_sys);
    run(rotation_update_sys);
    run(pause_sys, dt);
    run(twister_state_sys);
    run(state_transition_cleanup_sys);
    run(bullet_stream_sys, dt);
    run(invincibility_sys, dt);
    run(quadtree_update_sys);
    run(collision_sys);
    run(damage_sys);
    run(destruct_sys);
    run(draw_sys);
    // run(draw_collision_geometry_sys);

    auto post_run = SDL_GetPerformanceCounter();
    const float run_time = static_cast<float>(post_run - pre_run) / SDL_GetPerformanceFrequency();
    if (m_frames_elapsed % 50 == 0)
        std::cout << "frame compute time: " << run_time << std::endl;

    GPU_Flip(get_graphics_context()->renderer);

    quitting |= !get_manager()->has<Entity>(playerUUID());

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
    auto CM = get_manager();

    CM->unRegisterComponent<Entity>();
    CM->unRegisterComponent<EulerTranslation>();
    CM->unRegisterComponent<EulerRotation>();
    CM->unRegisterComponent<OffscreenBehavior>();
    CM->unRegisterComponent<DestructTag>();
    CM->unRegisterComponent<BulletTag>();
    CM->unRegisterComponent<FriendlyTag>();
    CM->unRegisterComponent<CollisionData>();
    CM->unRegisterComponent<PositionUpdate>();
    CM->unRegisterComponent<RotationUpdate>();
    CM->unRegisterComponent<Sprite>();
    CM->unRegisterComponent<StateTransition>();
    CM->unRegisterComponent<Twister::Tag>();
    CM->unRegisterComponent<TranslationSpline>();
    CM->unRegisterComponent<PauseBehavior>();
    CM->unRegisterComponent<BulletStream>();
    CM->unRegisterComponent<DamageEvent>();
    CM->unRegisterComponent<Health>();
    CM->unRegisterComponent<CollideDamage>();
    CM->unRegisterComponent<Invincibility>();
}
