#pragma once

#include "Base.hpp"
#include <SDL2/SDL.h>

#include <utility>

struct GraphicsContext {
    GraphicsContext(void);
    ~GraphicsContext(void);

    const int m_screenWidth;
    const float m_screenWidthFloat;
    SDL_Renderer* renderer;
    SDL_Window* window;

    Sint16 toScreenSpan(const float gridSpan) const;
    ScreenCoordinates toScreenCoordinates(const Position& pos) const;
};
