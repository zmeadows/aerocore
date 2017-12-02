#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "BoundingSurface.hpp"
#include "Geometry.hpp"
#include "GraphicsContext.hpp"

struct Sprite {
    virtual void draw(GraphicsContext* GC, const Position& pos, const Rotation& rot) const = 0;
    virtual BoundingSurface* buildBoundingSurface(void) const = 0;
    virtual ~Sprite(void) {}
};

struct SimplePolygonSprite : public Sprite {
    RGBA rgba;

    virtual void draw(GraphicsContext* GC, const Position& pos, const Rotation& rot) const = 0;

    virtual const PolygonShape& getPolygon(void) const = 0;
    virtual PolygonShape& getPolygon(void) = 0;

    BoundingSurface* buildBoundingSurface(void) const final
    {
        return new PolygonalBoundingSurface(getPolygon());
    }

    virtual ~SimplePolygonSprite(void) {}
};

class SquareSprite final : public SimplePolygonSprite
{
    SquareShape square;

public:
    SquareSprite(const float width)
        : square(width)
    {
    }

    const PolygonShape& getPolygon(void) const final { return square; }
    PolygonShape& getPolygon(void) final { return square; }

    void draw(GraphicsContext* GC, const Position& pos, const Rotation& rot) const final;

    ~SquareSprite() {}
};

class IsoTriangleSprite final : public SimplePolygonSprite
{
    IsoTriangleShape triangle;

public:
    IsoTriangleSprite(float baseWidth, float height)
        : triangle(baseWidth, height)
    {
    }

    const PolygonShape& getPolygon(void) const final { return triangle; }
    PolygonShape& getPolygon(void) final { return triangle; }

    void draw(GraphicsContext* GC, const Position& pos, const Rotation& rot) const final;

    ~IsoTriangleSprite() {}
};