#pragma once

#include "Base.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_gpu.h>

#include <utility>

struct ScreenCoordinates {
    Sint16 x = 0;
    Sint16 y = 0;
};

class GraphicsContext {
    const int screen_width;
    const float screen_width_f;

public:
    GraphicsContext(void);
    ~GraphicsContext(void);

    GPU_Target* renderer;

    Sint16 to_screen_span(const float grid_span) const;
    ScreenCoordinates to_screen_coords(const v2& pos) const;
};
