#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "Base.hpp"
#include "Sprite.hpp"
#include "BoundingSurface.hpp"
#include "GraphicsContext.hpp"

struct Sprite {
    std::vector<Vector2f> vertices = {};
    RGBA color = { 255, 255, 255, 255 };
    bool filled = false;
};

//TODO: struct SpritePool

std::vector<ScreenCoordinates> vtxToScreenCoords(GraphicsContext* GC, 
                                                 const std::vector<Vector2f>& vertices);

Sprite makeIsoTriangleSprite(float baseWidth, float height);
Sprite makeSquareSprite(float width);

std::vector<Vector2f> transformVtxs(const std::vector<Vector2f> vertices,
                                    const Position& pos, const Rotation& rot);

void draw(GraphicsContext* GC, const Sprite& sprite,
          const Position& pos, const Rotation& rot);

void scale(Sprite& sprite, float factor);

void extentAt(Extent& ext, const Sprite& sprite, const Position& pos, const Rotation& rot);

bool isOffScreen(const Sprite& sprite, const Position& pos, const Rotation& rot);

/*
bool overlaps(const Sprite& bsA,
              const Position& posA,
              const Rotation& rotA,
              const Sprite& bsB,
              const Position& posB,
              const Rotation& rotB) {
    // combine surface normals into one SurfaceNormalSet so that
    // duplicates are removed, rather than separately looping over both sets
    SurfaceNormalSet combinedSurfaceNormals;
    combinedSurfaceNormals.add(SurfaceNormalSet(bsA.vertices()), rotA.getAngle());
    combinedSurfaceNormals.add(SurfaceNormalSet(bsB.vertices()), rotB.getAngle());

    for (const Vector2f& axis : combinedSurfaceNormals) {
        AxisProjection projA = bsA.projectOn(axis, posA, rotA);
        AxisProjection projB = bsB.projectOn(axis, posB, rotB);
        if (!(projA.max >= projB.min && projB.max >= projA.min)) {
            return false;
        }
    }

    return true;
}

AxisProjection
projectOn(const Sprite& sprite, const Vector2f& axis, const Position& pos, const Rotation& rot) const {
    float minProjection = std::numeric_limits<float>::max();
    float maxProjection = std::numeric_limits<float>::lowest();

    for (const Vector2f& vtx : polygon.getTransRotVertices(pos, rot)) {
        const float projection = vtx.dot(axis);
        minProjection = std::min(projection, minProjection);
        maxProjection = std::max(projection, maxProjection);
    }

    return AxisProjection({minProjection, maxProjection});
}
*/
