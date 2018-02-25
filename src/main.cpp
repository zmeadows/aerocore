#include <SDL2/SDL.h>

#include <iostream>
#include <memory>
#include <regex>
#include <time.h>
#include <vector>
#include <string>
#include <stdlib.h>

#include "Game.hpp"
#include "Util.hpp"



int main(/* int argc, char** argv */) {
    srand (static_cast<unsigned>(time(nullptr)));
    auto game = std::make_unique<Game>();
    game->run();

    return 0;
}
