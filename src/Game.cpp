#include "Game.hpp"

#include "Globals.hpp"
#include "Generator/Player.hpp"
#include "Generator/Enemy.hpp"
#include "GraphicsContext.hpp"
#include "InputManager.hpp"
#include "SystemManager.hpp"
#include "System/MotionSystem.hpp"
#include "System/DrawSystem.hpp"
#include "System/AsteroidShardSystem.hpp"
#include "System/StabberSystem.hpp"
#include "QuadTreeDraw.hpp"

#include <SDL2/SDL.h>

Game::Game(void)
    : SM(std::make_unique<SystemManager>()),
      IM(std::make_unique<InputManager>())
{
    // TODO: add debug mode check in aerocore that components have
    // actually been registered. Right now it just segfaults.
    auto CM = get_manager();

    CM->registerComponent<Entity>(10000);
    CM->registerComponent<ShotDelay>(10000);
    CM->registerComponent<AsteroidShardData>(1000);
    CM->registerComponent<StabberData>(10000);

    SM->addSystem(new DrawSystem());
    SM->addSystem(new MotionSystem());
    SM->addSystem(new StabberSystem());
    SM->addSystem(new AsteroidShardSystem());

    generatePlayer();
}

bool Game::tick(void) {

    bool paused_before_user_input = paused;
    bool quitting = processInput();


    static Uint64 t0 = SDL_GetPerformanceCounter();

    auto GC = get_graphics_context();
    if (!paused) {
        GPU_ClearRGB(GC->renderer, 50, 50, 50);
    }

    Uint32 tmp_ticks = SDL_GetTicks();
    if (!paused && ((last_asteroid_time == 0) || tmp_ticks > last_asteroid_time + 2000)) {
        last_asteroid_time = SDL_GetTicks();
        generateOffscreenAsteroid();
        generateStabber();
    }

    if (!paused && !paused_before_user_input) {
        m_preSystemRunTicks = SDL_GetPerformanceCounter();
        SM->runSystems(static_cast<float>(SDL_GetPerformanceCounter() - t0) / SDL_GetPerformanceFrequency());
        m_postSystemRunTicks = SDL_GetPerformanceCounter();
        t0 = SDL_GetPerformanceCounter();
    } else if (!paused && paused_before_user_input){
        t0 = SDL_GetPerformanceCounter();
    }

    // if (m_frames_elapsed % 30 == 0)
    //     std::cout << "system update time: " << static_cast<float>(m_postSystemRunTicks - m_preSystemRunTicks) / SDL_GetPerformanceFrequency() << std::endl;

    m_preFlipTicks = SDL_GetPerformanceCounter();
    GPU_Flip(GC->renderer);
    m_postFlipTicks = SDL_GetPerformanceCounter();

    // if (m_frames_elapsed % 60 == 0)
    //     std::cout << "buffer flip time: " << static_cast<float>(m_postFlipTicks - m_preFlipTicks) / SDL_GetPerformanceFrequency() << std::endl;

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
        }
    }

    return false;
}

Game::~Game(void) {
    auto CM = get_manager();

    CM->unRegisterComponent<Entity>();
    CM->unRegisterComponent<ShotDelay>();
    CM->unRegisterComponent<AsteroidShardData>();
    CM->unRegisterComponent<StabberData>();
}
