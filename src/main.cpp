#include <SDL2/SDL.h>

#include "Aerocore.hpp"

int main(int argc, char** argv)
{
    Aerocore game;
    while (!game.tick())
        continue;
    return EXIT_SUCCESS;
}
