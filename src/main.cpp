#include <SDL2/SDL.h>

#include <iostream>
#include <memory>
#include <time.h>
#include <stdlib.h>

#include "Game.hpp"

int main(/* int argc, char** argv */) {
    srand (time(NULL));
    auto game = std::make_unique<Game>();
    game->run();

    return 0;
}
