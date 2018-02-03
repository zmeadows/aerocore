#include "Game.hpp"

#include <array>

#include "ComponentManager.hpp"
#include "Generator.hpp"
#include "GraphicsContext.hpp"
#include "InputManager.hpp"
#include "SystemManager.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

Game::Game(void)
    : GC(std::make_unique<GraphicsContext>()), CM(std::make_unique<ComponentManager>()),
      SM(std::make_unique<SystemManager>()), IM(std::make_unique<InputManager>(CM.get())) {
    SM->addSystem(new TranslationSystem(CM.get()));
    SM->addSystem(new RotationSystem(CM.get()));
    SM->addSystem(new CollisionSystem(CM.get()));
    SM->addSystem(new ParticleSystem(CM.get()));
    SM->addSystem(new CleanupSystem(CM.get()));
    SM->addSystem(new DrawSystem(CM.get(), GC.get()));
    generate<EntityType::Player>(CM.get());
    generate<EntityType::Enemy>(CM.get());
}

bool Game::tick(void) {
    bool quitting = processInput();

    static Uint64 t0 = SDL_GetPerformanceCounter();
    static Uint64 t1 = 0;

    SDL_SetRenderDrawColor(GC->renderer, 10, 10, 10, 255);
    SDL_RenderClear(GC->renderer);

    t1 = SDL_GetPerformanceCounter();
    const float tmpdt = (float)(t1 - t0) / SDL_GetPerformanceFrequency();
    SM->runSystems(tmpdt);
    t0 = SDL_GetPerformanceCounter();

    SDL_RenderPresent(GC->renderer);

    if (!quitting) {
        quitting = !CM->has<Position>(UUID::playerUUID);
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
