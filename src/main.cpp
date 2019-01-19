#include <SDL.h>
#include <time.h>
#include "Game.hpp"

int main(int argc, char** argv) {
    srand (static_cast<unsigned>(time(nullptr)));

    Game g;
    g.run();

    return 0;
}
