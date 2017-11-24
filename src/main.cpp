#include <SDL2/SDL.h>

#include "Base.hpp"
#include "ComponentManager.hpp"
#include "Generator.hpp"
#include "GraphicsContext.hpp"
#include "InputManager.hpp"
#include "Sprite.hpp"
#include "System.hpp"
#include "SystemManager.hpp"
#include "UUID.hpp"

int main(int argc, char** argv) {

    auto GC = std::make_unique<GraphicsContext>();
    auto CM = std::make_unique<ComponentManager>();
    auto SM = std::make_unique<SystemManager>(CM.get(), GC.get());
    auto IM = std::make_unique<InputManager>(CM.get());

    generate<EntityType::Player>(CM.get());
    generate<EntityType::Enemy>(CM.get());

    SDL_Event e;

    bool quit = false;

    Uint64 t1, t0 = SDL_GetPerformanceCounter();

    while (!quit) {

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
                IM->processInput(e.key.keysym.sym, false);
            } else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
                IM->processInput(e.key.keysym.sym, true);
            }
        }

        SDL_SetRenderDrawColor(GC->renderer, 225, 225, 225, 255);
        SDL_RenderClear(GC->renderer);
        t1 = SDL_GetPerformanceCounter();
        SM->runSystems((double)(t1 - t0) / SDL_GetPerformanceFrequency());
        t0 = SDL_GetPerformanceCounter();
        SDL_RenderPresent(GC->renderer);
    }

    return EXIT_SUCCESS;
}
