#include "Game.hpp"

#include "Globals.hpp"
#include "Generator/Player.hpp"
#include "Generator/Enemy.hpp"
#include "GraphicsContext.hpp"
#include "InputManager.hpp"
#include "AudioContext.hpp"

#include "System/DrawSystem.hpp"
//#include "System/StabberSystem.hpp"
#include "System/EulerTranslationSystem.hpp"
#include "System/EulerRotationSystem.hpp"
#include "System/OffscreenBehaviorSystem.hpp"
#include "System/DestructSystem.hpp"
#include "System/CollisionSystem.hpp"
#include "System/VertexBufferSystem.hpp"
#include "System/PositionUpdateSystem.hpp"
#include "System/RotationUpdateSystem.hpp"
#include "System/SoundSystem.hpp"
#include "System/LinearPathSystem.hpp"

#include "Entity/Twister.hpp"

#include "QuadTreeDraw.hpp"
#include "Random.hpp"

#include <SDL.h>

Game::Game(void) : IM(std::make_unique<InputManager>())
{
    // TODO: add debug mode check in aerocore that components have
    // actually been registered. Right now it just segfaults.
    auto CM = get_manager();

    CM->registerComponent<Entity>(10000);
    //CM->registerComponent<Stabber>(10000);
    CM->registerComponent<EulerTranslation>(10000);
    CM->registerComponent<EulerRotation>(10000);
    CM->registerComponent<OffscreenBehavior>(10000);
    CM->registerComponent<DestructTag>(10000);
    CM->registerComponent<FriendlyTag>(10000);
    CM->registerComponent<CollisionData>(10000);
    CM->registerComponent<PositionUpdate>(10000);
    CM->registerComponent<RotationUpdate>(10000);
    CM->registerComponent<SoundEvent>(10000);
    CM->registerComponent<Sprite>(10000);
    CM->registerComponent<LinearPath>(10000);
    CM->registerComponent<Twister::Data>(10000);

    //@NOTE: Order is important here!
    this->systems.emplace_back(new EulerTranslationSystem());
    this->systems.emplace_back(new EulerRotationSystem());
    this->systems.emplace_back(new LinearPathSystem());
    this->systems.emplace_back(new OffscreenBehaviorSystem());
    this->systems.emplace_back(new PositionUpdateSystem());
    this->systems.emplace_back(new RotationUpdateSystem());
    this->systems.emplace_back(new VertexBufferSystem());
    this->systems.emplace_back(new CollisionSystem());
    this->systems.emplace_back(new SoundSystem());
    this->systems.emplace_back(new DestructSystem());
    //this->systems.emplace_back(new StabberSystem());
    this->systems.emplace_back(new DrawSystem());
    this->systems.emplace_back(new Twister::System());

    generatePlayer();
}

bool Game::tick(void) {
    bool quitting = processInput();
    auto GC = get_graphics_context();

    if (SDL_GetTicks() > last_asteroid_time + 30) {
        Twister::generate( {uniform_random(-100, 100), 130.f } );
        Twister::generate( {uniform_random(-100, 100), 130.f } );
        last_asteroid_time = SDL_GetTicks();
    }

    m_preSystemRunTicks = SDL_GetPerformanceCounter();
    const float frame_time = static_cast<float>(m_preSystemRunTicks - m_postSystemRunTicks) / SDL_GetPerformanceFrequency();

    for (auto& sys : systems) {
        sys->run(frame_time);
    }

    m_postSystemRunTicks = SDL_GetPerformanceCounter();

    GPU_Flip(GC->renderer);

    quitting |= !get_manager()->has<Entity>(playerUUID());

    m_frames_elapsed++;

    return quitting;
}

bool Game::processInput(void) {
    SDL_Event e;

    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT)
            return true;

        if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                paused = !paused;
            } else if (!paused) {
                IM->processInput(e.key.keysym.sym, false);
            }
        } else if (!paused && e.type == SDL_KEYUP && e.key.repeat == 0) {
            IM->processInput(e.key.keysym.sym, true);
        } else if (e.type == SDL_JOYAXISMOTION && e.jaxis.which == 0 ) {
            if ( e.jaxis.axis == 0 ) IM->updateGamepadJoystickX(e.jaxis.value);
            if ( e.jaxis.axis == 1 ) IM->updateGamepadJoystickY(e.jaxis.value);
            IM->processJoystickInput();
        } else if (e.type == SDL_JOYBUTTONDOWN && e.jaxis.which == 0) {
            IM->processGamepadButtonInput(e.jbutton.button, e.jbutton.state == SDL_PRESSED);
        }
    }

    return false;
}

Game::~Game(void) {
    auto CM = get_manager();

    CM->unRegisterComponent<Entity>();
    // CM->unRegisterComponent<Stabber>();
    CM->unRegisterComponent<EulerTranslation>();
    CM->unRegisterComponent<EulerRotation>();
    CM->unRegisterComponent<OffscreenBehavior>();
    CM->unRegisterComponent<DestructTag>();
}
