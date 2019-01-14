#include "GraphicsContext.hpp"

#include "Util.hpp"
#include "Vector2D.hpp"

#include <iostream>

GraphicsContext::GraphicsContext(void) : screen_width(800), screen_width_f(800.f) {
    SDL_Init(SDL_INIT_EVERYTHING);

    // window = SDL_CreateWindow("aerocore",
    //                           SDL_WINDOWPOS_CENTERED,
    //                           SDL_WINDOWPOS_CENTERED,
    //                           screen_width,
    //                           screen_width,
    //                           SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);

    // SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    // SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);

    // GPU_SetPreInitFlags(GPU_INIT_DISABLE_VSYNC);
    renderer = GPU_Init(screen_width, screen_width, GPU_DEFAULT_INIT_FLAGS);

    if (SDL_NumJoysticks() < 1) {
        std::cout << "Warning: No joysticks connected!" << std::endl;
    } else {
        gamepad = SDL_JoystickOpen(0);
        if (!gamepad) {
            std::cout << "Failed to load game pad!" << std::endl;
        }
    }

    // gfxPrimitivesSetFont(nullptr, 0, 0);
}

GraphicsContext::~GraphicsContext(void) {
    DEBUG("Destroying GraphicsContext...");
    SDL_JoystickClose(gamepad);
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
