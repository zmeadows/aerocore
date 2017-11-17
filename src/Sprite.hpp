#pragma once

#include <SDL2/SDL.h>

class Sprite {
public:
    virtual void draw(SDL_Renderer* renderer, int x, int y) = 0;
};

struct RGBA {
  uint_fast16_t r = 0;
  uint_fast16_t g = 0;
  uint_fast16_t b = 0;
  uint_fast16_t a = 0;
};

class ColoredSprite : public Sprite {
public:
  RGBA rgba;

  virtual void draw(SDL_Renderer* renderer, int x, int y)  = 0;
};

class SquareSprite final : public ColoredSprite {
private:
    SDL_Rect rect;
public:
    SquareSprite(const int w) {
        rect.w = w;
        rect.h = w;
    }

    void draw(SDL_Renderer* renderer, int x, int y) final {
        rect.x = x;
        rect.y = y;
        SDL_SetRenderDrawColor(renderer, rgba.r, rgba.g, rgba.b, rgba.a);
        SDL_RenderFillRect(renderer, &rect);
    }
};