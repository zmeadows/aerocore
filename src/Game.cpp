#include "Game.hpp"

#include <array>

#include "Generator/Player.hpp"
#include "Generator/Enemy.hpp"
#include "GraphicsContext.hpp"
#include "InputManager.hpp"
#include "SystemManager.hpp"
#include "System/MotionSystem.hpp"
#include "System/CollisionSystem.hpp"
#include "System/DrawSystem.hpp"
#include "QuadTreeDraw.hpp"

#include "aerocore.hpp"

#include <SDL2/SDL.h>

Game::Game(void)
    : CM(std::make_unique<ComponentManager>()),
      GC(std::make_unique<GraphicsContext>()),
      SM(std::make_unique<SystemManager>()),
      m_quadTree(std::make_unique<QuadTree>(4)),
      IM(std::make_unique<InputManager>(CM.get(), m_quadTree.get()))
{
    // TODO: add check in aerocore that components have
    // actually been registered. Right now it just segfaults.
    CM->registerComponent<CoreData>(10000);
    CM->registerComponent<Alliance>(10000);
    CM->registerComponent<OffscreenBehavior>(10000);
    CM->registerComponent<ShotDelay>(10000);
    CM->registerComponent<DeathTimer>(10000);
    CM->registerComponent<ParticleGenerator>(10);
    CM->registerComponent<CollisionData>(10000);

    SM->addSystem(new MotionSystem(CM.get(), m_quadTree.get()));
    SM->addSystem(new CollisionSystem(CM.get()));
    SM->addSystem(new ParticleSystem(CM.get()));
    SM->addSystem(new CleanupSystem(CM.get()));
    SM->addSystem(new DrawSystem(CM.get(), GC.get()));
    generatePlayer(CM.get(), m_quadTree.get());

    void drawQuadtree(void);
}

bool Game::tick(void) {
    bool quitting = processInput();

    static Uint64 t0 = SDL_GetPerformanceCounter();
    // static Uint64 t1 = 0;

    GPU_ClearRGB(GC->renderer, 30, 30, 30);

    Uint32 tmp_ticks = SDL_GetTicks();
    if (last_asteroid_time == 0 || tmp_ticks > last_asteroid_time + 1000) {
        last_asteroid_time = SDL_GetTicks();
        generateOffscreenAsteroid(CM.get(), m_quadTree.get());
    }

    drawQuadTree(GC.get(), m_quadTree.get());
    SM->runSystems(static_cast<float>(SDL_GetPerformanceCounter() - t0) / SDL_GetPerformanceFrequency());
    t0 = SDL_GetPerformanceCounter();

    GPU_Flip(GC->renderer);

    if (!quitting) {
        quitting = !CM->has<CoreData>(playerUUID());
    }

    return quitting;
}

bool Game::processInput(void) {
    SDL_Event e;

    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            return true;
        } else if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
            IM->processInput(e.key.keysym.sym, false);
            return false;
        } else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
            IM->processInput(e.key.keysym.sym, true);
            return false;
        }
    }

    return false;
}

Game::~Game(void) {
    CM->unRegisterComponent<CoreData>();
    CM->unRegisterComponent<Alliance>();
    CM->unRegisterComponent<OffscreenBehavior>();
    CM->unRegisterComponent<ShotDelay>();
    CM->unRegisterComponent<DeathTimer>();
    CM->unRegisterComponent<ParticleGenerator>();
    CM->unRegisterComponent<CollisionData>();
}
