#include "Game.hpp"

#include <array>

#include "Globals.hpp"
#include "Generator/Player.hpp"
#include "Generator/Enemy.hpp"
#include "GraphicsContext.hpp"
#include "InputManager.hpp"
#include "SystemManager.hpp"
#include "System/MotionSystem.hpp"
#include "System/CollisionSystem.hpp"
#include "System/DrawSystem.hpp"
#include "System/AsteroidShardSystem.hpp"
#include "System/StabberSystem.hpp"
#include "QuadTreeDraw.hpp"

#include "aerocore.hpp"

#include <SDL2/SDL.h>

Game::Game(void)
    : SM(std::make_unique<SystemManager>()),
      IM(std::make_unique<InputManager>())
{
    // TODO: add debug mode check in aerocore that components have
    // actually been registered. Right now it just segfaults.
    auto CM = get_manager();

    CM->registerComponent<Entity>(10000);
    CM->registerComponent<OffscreenBehavior>(10000);
    CM->registerComponent<ShotDelay>(10000);
    CM->registerComponent<DeathTimer>(10000);
    CM->registerComponent<ParticleGenerator>(10);
    CM->registerComponent<CollisionData>(10000);
    CM->registerComponent<AsteroidShardData>(1000);
    CM->registerComponent<StabberData>(10000);

    SM->addSystem(new MotionSystem());
    SM->addSystem(new CollisionSystem());
    SM->addSystem(new AsteroidShardSystem());
    SM->addSystem(new StabberSystem());
    SM->addSystem(new ParticleSystem());
    SM->addSystem(new CleanupSystem());
    SM->addSystem(new DrawSystem());

    generatePlayer();
}

bool Game::tick(void) {

    bool paused_before_user_input = paused;
    bool quitting = processInput();


    static Uint64 t0 = SDL_GetPerformanceCounter();
    // static Uint64 t1 = 0;

    auto GC = get_graphics_context();
    if (!paused) {
        GPU_ClearRGB(GC->renderer, 30, 30, 30);
    }

    Uint32 tmp_ticks = SDL_GetTicks();
    if (!paused && (last_asteroid_time == 0) || tmp_ticks > last_asteroid_time + 399) {
        generateStabber();
        last_asteroid_time = SDL_GetTicks();
        generateOffscreenAsteroid();
    }
    //     last_asteroid_time = SDL_GetTicks();
    // }

    // drawQuadTree(get_graphics_context(), get_quad_tree());
    if (!paused && !paused_before_user_input) {
        SM->runSystems(static_cast<float>(SDL_GetPerformanceCounter() - t0) / SDL_GetPerformanceFrequency());
        t0 = SDL_GetPerformanceCounter();
    } else if (!paused && paused_before_user_input){
        // @IDEA : diff gamestate after running systems with dt = 0 to check for bugs
        t0 = SDL_GetPerformanceCounter();
    }
        GPU_Flip(GC->renderer);

    quitting |= !get_manager()->has<Entity>(playerUUID());

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
    CM->unRegisterComponent<OffscreenBehavior>();
    CM->unRegisterComponent<ShotDelay>();
    CM->unRegisterComponent<DeathTimer>();
    CM->unRegisterComponent<ParticleGenerator>();
    CM->unRegisterComponent<CollisionData>();
    CM->unRegisterComponent<AsteroidShardData>();
    CM->unRegisterComponent<StabberData>();
}
