#pragma once

#include <SDL.h>
#include <SDL_gpu.h>
// #include <NFont/NFont.h>

struct v2;

struct ScreenCoordinates {
    Sint16 x = 0;
    Sint16 y = 0;
};

class GraphicsContext {
    const Uint16 screen_width;
    const float screen_width_f;

    SDL_GameController* gamepad;

    // NFont font;

public:
    GraphicsContext(void);
    ~GraphicsContext(void);

    GPU_Target* renderer;

    Sint16 to_screen_span(const float grid_span) const;
    ScreenCoordinates to_screen_coords(const v2& pos) const;
};
