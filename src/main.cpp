#include <SDL2/SDL.h>

#include "Aerocore.hpp"

int main(int argc, char** argv)
{

    bool quit = false;

    Aerocore game;

    while (!quit) {
        quit = game.tick();
    }

    return EXIT_SUCCESS;
}
