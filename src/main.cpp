#include <SDL2/SDL.h>

#include "Game.hpp"

int main(int argc, char** argv) {
    Game game;
    game.run();

    std::exit(EXIT_SUCCESS);
}
