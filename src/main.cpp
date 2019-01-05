#include <SDL.h>

#include <iostream>
#include <memory>
#include <regex>
#include <time.h>
#include <vector>
#include <string>
#include <stdlib.h>

#include "Game.hpp"
#include "Util.hpp"
#include "Spline.hpp"
#include "random.hpp"

int main(int argc, char** argv) {
    srand (static_cast<unsigned>(time(nullptr)));

    for (auto i = 0; i < 200; i++) {
        std::cout << uniform_random(0, 1) << std::endl;
    }

    // std::vector<v2> ps = { {0,0}, {10,10}, {0, 12}, {10, -10}, {0,-4} };
    // std::vector<f32> ts = { 0.0, 0.5, 1.3, 3.0, 5.0 };

    // auto s = make_spline(ps, ts);

    // while (true) {
    //     SplineStepResult result = s.step(0.05, false);

    //     std::cout << result.new_point.x << " " << result.new_point.y << std::endl;

    //     if (result.unused_dt > 0)
    //         break;
    // }


    auto game = std::make_unique<Game>();
    game->run();

    return 0;
}
