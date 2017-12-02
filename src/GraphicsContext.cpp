#include "GraphicsContext.hpp"

#include <iostream>

GraphicsContext::GraphicsContext(void)
    : m_screenWidth(800)
    , m_screenWidthFloat(800.0)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow("aerocore", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_screenWidth,
                              m_screenWidth, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_GL_SetSwapInterval(1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);

    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
}

GraphicsContext::~GraphicsContext(void)
{
    DEBUG("Destroying GraphicsContext...");
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

Sint16 GraphicsContext::toScreenSpan(const float gridSpan) const
{
    return static_cast<int>(nearbyint(m_screenWidthFloat * gridSpan / 200.0));
}

ScreenCoordinates GraphicsContext::toScreenCoordinates(const Position& pos) const
{
    Sint16 x = static_cast<int>(nearbyint(m_screenWidthFloat * pos.x / 200.0 + m_screenWidthFloat / 2.0));

    Sint16 y = static_cast<int>(nearbyint(m_screenWidthFloat * pos.y / (-200.0) + m_screenWidthFloat / 2.0));

    return {x, y};
}