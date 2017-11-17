#include <SDL2/SDL.h>

#include "UUID.hpp"
#include "ComponentManager.hpp"
#include "SystemManager.hpp"
#include "InputManager.hpp"
#include "System.hpp"
#include "Sprite.hpp"

int main (int argc, char** argv)
{
    SDL_Init( SDL_INIT_VIDEO );

    SDL_Window* window = nullptr;

    window = SDL_CreateWindow(
      "aerocore",
      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800,
      SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALLOW_HIGHDPI
    );
    
    // Setup renderer
    SDL_Renderer* renderer = nullptr;
    renderer =  SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    auto CM = std::make_unique<ComponentManager>();
    auto SM = std::make_unique<SystemManager>(CM.get(), renderer);
    auto IM = std::make_unique<InputManager>(CM.get());

    CM->add<Position>(UUID::playerUUID, {50,50});
    CM->add<Velocity>(UUID::playerUUID, {0,0});
    CM->add<Sprite>(UUID::playerUUID, new SquareSprite(50));

    SDL_Event e;

    bool quit = false;

    while (!quit) {
      while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
          quit = true;
        } else if (e.type == SDL_KEYDOWN) {
            IM->processInput(e.key.keysym.sym, false);
        } else if (e.type == SDL_KEYUP) {
            IM->processInput(e.key.keysym.sym, true);
        }
      }

      SDL_SetRenderDrawColor(renderer, 25, 25, 25, 255);
      SDL_RenderClear(renderer);
      SM->runSystems();
      SDL_RenderPresent(renderer);
      SDL_Delay(10);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
