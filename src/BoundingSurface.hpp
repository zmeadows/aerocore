#pragma once

#include <cassert>
#include <limits>
#include <math.h>
#include <memory>
#include <vector>

#include <SDL2/SDL2_gfxPrimitives.h>

#include "wykobi.hpp"

#include "Base.hpp"
#include "Geometry.hpp"
#include "GraphicsContext.hpp"
#include "Vector2D.hpp"

struct AxisProjection {
    const float min = 0.f;
    const float max = 0.f;
};

class SurfaceNormalSet {
private:
    std::vector<Vector2f> normals;

public:
    SurfaceNormalSet(const std::vector<Vector2f>& vertices);
    SurfaceNormalSet(const SurfaceNormalSet& rhs);
    SurfaceNormalSet(void) {}

    void add(const Vector2f& vec, float rotationAngle = 0.f);
    void add(const SurfaceNormalSet& rhs, float rotationAngle = 0.f);
    void add(const SurfaceNormalSet* rhs, float rotationAngle = 0.f);

    std::vector<Vector2f>::const_iterator begin(void) const { return normals.begin(); }
    std::vector<Vector2f>::const_iterator end(void) const { return normals.end(); }

    inline size_t size(void) const { return normals.size(); }
};

struct BoundingSurface {
    virtual AxisProjection
    projectOn(const Vector2f& axis, const Position& pos, const Rotation& rot) const = 0;

    virtual const SurfaceNormalSet* getSurfaceNormals(void) const = 0;

    virtual void draw(GraphicsContext* GC, const Position& pos, const Rotation& rot) const = 0;

    virtual ~BoundingSurface() {}
};

class PolygonalBoundingSurface final : public BoundingSurface {
private:
    PolygonShape polygon;
    SurfaceNormalSet normals;

public:
    explicit PolygonalBoundingSurface(const PolygonShape& polygon_)
        : polygon(polygon_), normals(polygon.getVertices()) {}

    PolygonalBoundingSurface(void) = delete;
    PolygonalBoundingSurface& operator=(const PolygonalBoundingSurface&) = delete;
    PolygonalBoundingSurface& operator=(PolygonalBoundingSurface&&) = delete;
    PolygonalBoundingSurface(PolygonalBoundingSurface&&) = delete;

    virtual ~PolygonalBoundingSurface(void) {}

    const SurfaceNormalSet* getSurfaceNormals(void) const final { return &normals; }

    void draw(GraphicsContext* GC, const Position& pos, const Rotation& rot) const {
        // const std::vector<Vector2f> vertices = polygon.getTransRotVertices(pos, rot);

        // std::vector<ScreenCoordinates> vtx;

        // for (const auto& a : vertices) {
        //     vtx.push_back(GC->toScreenCoordinates({a.x, a.y}));
        // }

        // const size_t vtxCount = vtx.size();

        // for (size_t i = 0; i < vtxCount; i++) {
        //     aalineRGBA(GC->renderer, vtx[i].x, vtx[i].y, vtx[(i + 1) % vtxCount].x, vtx[(i + 1) %
        //     vtxCount].y,
        //                0, 0, 255, 125);
        // }
    }

    AxisProjection projectOn(const Vector2f& axis, const Position& pos, const Rotation& rot) const final;
};

// class GeneralPolygonBoundingSurface final : public BoundingSurface {
//
//     SurfaceNormalSet normals;
// };

bool overlaps(const BoundingSurface& bsA,
              const Position& posA,
              const Rotation& rotA,
              const BoundingSurface& bsB,
              const Position& posB,
              const Rotation& rotB);
