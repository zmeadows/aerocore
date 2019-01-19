#include "GraphicsContext.hpp"

#include "Util.hpp"
#include "Vector2D.hpp"

#include <iostream>

//WARNING: parse_svg_path will break when you change screen width
//other things might as well, grep files for '800'
GraphicsContext::GraphicsContext(void) : screen_width(800), screen_width_f(800.f) {
    SDL_Init(SDL_INIT_EVERYTHING);

    renderer = GPU_Init(screen_width, screen_width, GPU_DEFAULT_INIT_FLAGS);

    if (SDL_NumJoysticks() < 1) {
        std::cout << "Warning: No joysticks connected!" << std::endl;
    } else if (!SDL_IsGameController(0)) {
        std::cout << "Warning: No joysticks connected!" << std::endl;
    } else {
        gamepad = SDL_GameControllerOpen(0);
        if (!gamepad) {
            std::cout << "Failed to load game pad!" << std::endl;
        }
    }
}

GraphicsContext::~GraphicsContext(void) {
    DEBUG("Destroying GraphicsContext...");
    SDL_GameControllerClose(gamepad);
    GPU_Quit();
}

Sint16 GraphicsContext::to_screen_span(const float grid_span) const {
    // TODO: assert in window
    return static_cast<Sint16>(nearbyint(screen_width_f * grid_span / 200.f));
}

ScreenCoordinates GraphicsContext::to_screen_coords(const v2& pos) const {
    // TODO: assert in window
    Sint16 x = static_cast<Sint16>(nearbyint(screen_width_f * pos.x / 200.f + screen_width_f / 2.f));
    Sint16 y = static_cast<Sint16>(nearbyint(screen_width_f * pos.y / (-200.f) + screen_width_f / 2.f));

    return {x, y};
}
