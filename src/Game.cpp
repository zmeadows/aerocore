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
#include "Component/StateTransition.hpp"
#include "Component/OffscreenBehavior.hpp"
#include "Component/BulletStream.hpp"

#include "System/BulletStreamSystem.hpp"
#include "System/CollisionSystem.hpp"
#include "System/DamageSystem.hpp"
#include "System/DestructSystem.hpp"
#include "System/DrawCollisionGeometrySystem.hpp"
#include "System/DrawSystem.hpp"
#include "System/EulerRotationSystem.hpp"
#include "System/EulerTranslationSystem.hpp"
#include "System/InvincibilitySystem.hpp"
#include "System/LinearPathSystem.hpp"
#include "System/OffscreenBehaviorSystem.hpp"
#include "System/PositionUpdateSystem.hpp"
#include "System/QuadTreeUpdateSystem.hpp"
#include "System/RotationUpdateSystem.hpp"
#include "System/StateTransitionCleanupSystem.hpp"

#include "Behavior/Pause.hpp"

#include "QuadTreeDraw.hpp"
#include "Random.hpp"
#include "Spline.hpp"

#include <SDL.h>

Game::Game(void)
{
    // TODO: add debug mode check in aerocore that components have
    // actually been registered. Right now it just segfaults.
    auto CM = get_manager();

    CM->registerComponent<Entity>(10000);
    CM->registerComponent<EulerTranslation>(10000);
    CM->registerComponent<EulerRotation>(10000);
    CM->registerComponent<OffscreenBehavior>(10000);
    CM->registerComponent<DestructTag>(10000);
    CM->registerComponent<BulletTag>(10000);
    CM->registerComponent<FriendlyTag>(10000);
    CM->registerComponent<CollisionData>(10000);
    CM->registerComponent<PositionUpdate>(10000);
    CM->registerComponent<RotationUpdate>(10000);
    CM->registerComponent<Sprite>(10000);
    CM->registerComponent<StateTransition>(10000);
    CM->registerComponent<Twister::Tag>(10000);
    CM->registerComponent<TranslationSpline>(10000);
    CM->registerComponent<PauseBehavior>(10000);
    CM->registerComponent<BulletStream>(10000);
    CM->registerComponent<DamageEvent>(10000);
    CM->registerComponent<Health>(10000);
    CM->registerComponent<CollideDamage>(10000);
    CM->registerComponent<Invincibility>(10000);

    //@NOTE: Order is important here!

    this->systems.emplace_back(new EulerTranslationSystem());
    this->systems.emplace_back(new EulerRotationSystem());

    this->systems.emplace_back(new TranslationSplineSystem());

    this->systems.emplace_back(new OffscreenBehaviorSystem());
    this->systems.emplace_back(new PositionUpdateSystem());
    this->systems.emplace_back(new RotationUpdateSystem());


    this->systems.emplace_back(new PauseSystem());

    this->systems.emplace_back(new Twister::StateMachineSystem());
    this->systems.emplace_back(new StateTransitionCleanupSystem());

    this->systems.emplace_back(new BulletStreamSystem());

    this->systems.emplace_back(new InvincibilitySystem());
    this->systems.emplace_back(new QuadTreeUpdateSystem());
    this->systems.emplace_back(new CollisionSystem());

    this->systems.emplace_back(new DamageSystem());
    this->systems.emplace_back(new DestructSystem());

    this->systems.emplace_back(new DrawSystem());
    this->systems.emplace_back(new DrawCollisionGeometrySystem());

    generate_player();
    Twister::generate();
    last_asteroid_time = 0;
}

bool Game::tick(void) {
    m_frames_elapsed++;

    bool quitting = false;

    quitting = poll_input();

    if (SDL_GetTicks() > last_asteroid_time + 100) {
        Twister::generate();
        last_asteroid_time = SDL_GetTicks();
    }

    m_preSystemRunTicks = SDL_GetPerformanceCounter();
    const float frame_time = static_cast<float>(m_preSystemRunTicks - m_postSystemRunTicks) / SDL_GetPerformanceFrequency();
    time_elapsed += frame_time;
    m_postSystemRunTicks = m_preSystemRunTicks;

    tick_player(&input, &state, frame_time);

    for (auto& sys : systems) {
        sys->run(frame_time);
    }

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

    //TODO: use destructor in ResourceManager
    CM->unRegisterComponent<Entity>();
    CM->unRegisterComponent<EulerTranslation>();
    CM->unRegisterComponent<EulerRotation>();
    CM->unRegisterComponent<OffscreenBehavior>();
    CM->unRegisterComponent<DestructTag>();
}
