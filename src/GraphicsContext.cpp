#include "GraphicsContext.hpp"

#include "Util.hpp"
#include "Vector2D.hpp"

#include <cstdio>

//WARNING: parse_svg_path will break when you change screen width
//other things might as well, grep files for '800'
GraphicsContext::GraphicsContext(u16 init_screen_width, u16 init_screen_height)
    : screen_width(init_screen_width), screen_height(init_screen_height)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    renderer = GPU_Init(screen_width, screen_height, GPU_DEFAULT_INIT_FLAGS);
    // GPU_SetFullscreen(true, false);

    if (SDL_NumJoysticks() < 1) {
        printf("Warning: No joysticks connected!\n");
    } else if (!SDL_IsGameController(0)) {
        printf("Warning: Joystick is not a GameController!\n");
    } else {
        gamepad = SDL_GameControllerOpen(0);
        if (!gamepad) {
            printf("Error: Failed to load GameController\n");
        }
    }
}

GraphicsContext::~GraphicsContext(void) {
    DEBUG("Destroying GraphicsContext...");
    // SDL_GameControllerClose(gamepad);
    GPU_Quit();
}

ScreenDistance GraphicsContext::convert_to_screen_distance(GameDistance game_distance) const {
    const f32 dist = this->screen_width * game_distance.span / GameCoordinates::max();
    return { static_cast<Sint16>(nearbyint(dist)) };
}

ScreenCoordinates GraphicsContext::convert_to_screen_coords(GameCoordinates game_coordinates) const {
    static const f32 maxXpos = 16.f;
    static const f32 minYpos = -9.f;

    const Sint16 x = static_cast<Sint16>(nearbyint(this->screen_width * game_coordinates.x / maxXpos + this->screen_width / 2.f));
    const Sint16 y = static_cast<Sint16>(nearbyint(this->screen_height * game_coordinates.y / minYpos + this->screen_height / 2.f));

    assert(x <= this->screen_width);
    assert(y <= this->screen_height);
    return {x, y};
}
