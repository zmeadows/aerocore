#include "Aerocore.hpp"

#include <SDL2/SDL.h>
// #include "Base.hpp"
// #include "BoundingSurface.hpp"
#include "ComponentManager.hpp"
#include "Generator.hpp"
#include "GraphicsContext.hpp"
#include "InputManager.hpp"
// #include "Sprite.hpp"
// #include "System.hpp"
#include "SystemManager.hpp"
// #include "UUID.hpp"

Aerocore::Aerocore(void)
    : GC(std::make_unique<GraphicsContext>())
    , CM(std::make_unique<ComponentManager>())
    , SM(std::make_unique<SystemManager>(CM.get(), GC.get()))
    , IM(std::make_unique<InputManager>(CM.get()))
{
    generate<EntityType::Player>(CM.get());
    generate<EntityType::Enemy>(CM.get());
}

bool Aerocore::tick(void)
{
    bool quitting = processInput();

    static Uint64 t1 = 0, t0 = 0;

    SDL_SetRenderDrawColor(GC->renderer, 175, 175, 175, 0);
    SDL_RenderClear(GC->renderer);
    t1 = SDL_GetPerformanceCounter();
    SM->runSystems((float)(t1 - t0) / SDL_GetPerformanceFrequency());
    t0 = SDL_GetPerformanceCounter();
    SDL_RenderPresent(GC->renderer);

    return quitting;
}

bool Aerocore::processInput(void)
{
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