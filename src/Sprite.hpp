#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "BoundingSurface.hpp"
#include "GraphicsContext.hpp"

struct Sprite {
    virtual void draw(GraphicsContext* GC, const Position& pos) const = 0;
    virtual BoundingSurface* buildBoundingSurface(void) const = 0;

    virtual ~Sprite(void) {}
};

struct RGBA {
    uint_fast8_t r = 0;
    uint_fast8_t g = 0;
    uint_fast8_t b = 0;
    uint_fast8_t a = 0;
};

struct ColoredSprite : public Sprite {
    RGBA rgba;

    virtual void draw(GraphicsContext* GC, const Position& pos) const = 0;
    virtual BoundingSurface* buildBoundingSurface(void) const = 0;

    virtual ~ColoredSprite(void) {}
};

class SquareSprite final : public ColoredSprite
{
private:
    const float width;

public:
    SquareSprite(const float width_)
        : width(width_)
    {
    }

    void draw(GraphicsContext* GC, const Position& pos) const final;

    BoundingSurface* buildBoundingSurface(void) const final { return boundingSquare(width / 2.0); }

    ~SquareSprite() {}
};