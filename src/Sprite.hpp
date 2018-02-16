#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "Base.hpp"
#include "Sprite.hpp"
#include "BoundingSurface.hpp"
#include "Geometry.hpp"
#include "GraphicsContext.hpp"

struct NewSprite {
    std::vector<Vector2f> vertices = {};
    RGBA color = { 255, 255, 255, 255 };
    bool filled = false;
};

std::vector<ScreenCoordinates> vtxToScreenCoords(GraphicsContext* GC, 
                                                 const std::vector<Vector2f>& vertices);

NewSprite makeIsoTriangleSprite(float baseWidth, float height);
NewSprite makeSquareSprite(float width);

std::vector<Vector2f> transformVtxs(const std::vector<Vector2f> vertices,
                                    const Position& pos, const Rotation& rot);

void draw(GraphicsContext* GC, const NewSprite& sprite,
          const Position& pos, const Rotation& rot);

void scale(NewSprite& sprite, float factor);

void extentAt(Extent& ext, const NewSprite& sprite, const Position& pos, const Rotation& rot);

bool isOffScreen(const NewSprite& sprite, const Position& pos, const Rotation& rot);

/*
struct Sprite {
    virtual void draw(GraphicsContext* GC, const Position& pos, const Rotation& rot) const = 0;
    virtual BoundingSurface* buildBoundingSurface(void) const = 0;
    virtual bool isOffScreen(const Position& pos, const Rotation& rot) const = 0;
    virtual Extent extentAt(const Position& pos, const Rotation& rot) const = 0;
    virtual ~Sprite(void) {}
};

struct SpriteUpdator {
    std::function<void(Sprite*)> update;
};

struct SimplePolygonSprite : public Sprite {
    RGBA rgba;

    virtual void draw(GraphicsContext* GC, const Position& pos, const Rotation& rot) const = 0;

    virtual const PolygonShape& getPolygon(void) const = 0;
    virtual PolygonShape& getPolygon(void) = 0;

    BoundingSurface* buildBoundingSurface(void) const final {
        return new PolygonalBoundingSurface(getPolygon());
    }

    bool isOffScreen(const Position& pos, const Rotation& rot) const final {

        for (const auto& vtx : getPolygon().getTransRotVertices(pos, rot)) {
            if (std::abs(vtx.x) < 100.f && std::abs(vtx.y) < 100.f) {
                return false;
            }
        }

        return true;
    }

    Extent extentAt(const Position& pos, const Rotation& rot) const final {
        Extent ext;

        ext.minX = std::numeric_limits<float>::max();
        ext.maxX = std::numeric_limits<float>::lowest();
        ext.minY = std::numeric_limits<float>::max();
        ext.maxY = std::numeric_limits<float>::lowest();

        for (const auto& vtx : getPolygon().getTransRotVertices(pos, rot)) {
            ext.minX = std::min(ext.minX, vtx.x);
            ext.maxX = std::max(ext.maxX, vtx.x);
            ext.minY = std::min(ext.minY, vtx.y);
            ext.maxY = std::max(ext.maxY, vtx.y);
        }

        return ext;
    }

    virtual ~SimplePolygonSprite(void) {}
};

class SquareSprite final : public SimplePolygonSprite {
    SquareShape square;

public:
    SquareSprite(const float width) : square(width) {}

    const PolygonShape& getPolygon(void) const final { return square; }
    PolygonShape& getPolygon(void) final { return square; }

    void draw(GraphicsContext* GC, const Position& pos, const Rotation& rot) const final;

    ~SquareSprite() {}
};

class IsoTriangleSprite final : public SimplePolygonSprite {
    IsoTriangleShape triangle;

public:
    IsoTriangleSprite(float baseWidth, float height) : triangle(baseWidth, height) {}

    const PolygonShape& getPolygon(void) const final { return triangle; }
    PolygonShape& getPolygon(void) final { return triangle; }

    void draw(GraphicsContext* GC, const Position& pos, const Rotation& rot) const final;

    ~IsoTriangleSprite() {}
};
*/
