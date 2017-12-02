#pragma once

#include <cassert>
#include <limits>
#include <math.h>
#include <memory>
#include <vector>

#include <Eigen/Dense>

#include "Base.hpp"
#include "Geometry.hpp"

using namespace Eigen;

struct AxisProjection {
    float min = 0.f;
    float max = 0.f;
};

class SurfaceNormalSet
{
private:
    std::vector<Vector2f> normals;

public:
    SurfaceNormalSet(const std::vector<Vector2f>& vertices);
    SurfaceNormalSet(const SurfaceNormalSet& rhs);
    SurfaceNormalSet(void) {}

    void add(const Vector2f& vec);
    void add(const SurfaceNormalSet& rhs);
    void add(const SurfaceNormalSet* rhs);

    std::vector<Vector2f>::const_iterator begin(void) const { return normals.begin(); }
    std::vector<Vector2f>::const_iterator end(void) const { return normals.end(); }

    inline size_t size(void) const { return normals.size(); }
};

struct BoundingSurface {
    virtual AxisProjection
    projectOn(const Vector2f& axis, const Position& pos, const Rotation& rot) const = 0;

    virtual const SurfaceNormalSet* getSurfaceNormals(void) const = 0;

    virtual ~BoundingSurface() {}
};

class PolygonalBoundingSurface final : public BoundingSurface
{
private:
    PolygonShape polygon;
    SurfaceNormalSet normals;

public:
    explicit PolygonalBoundingSurface(const PolygonShape& polygon_)
        : polygon(polygon_)
        , normals(polygon.getVertices())
    {
    }

    PolygonalBoundingSurface(void) = delete;
    PolygonalBoundingSurface& operator=(const PolygonalBoundingSurface&) = delete;
    PolygonalBoundingSurface& operator=(PolygonalBoundingSurface&&) = delete;
    PolygonalBoundingSurface(PolygonalBoundingSurface&&) = delete;

    virtual ~PolygonalBoundingSurface(void) {}

    const SurfaceNormalSet* getSurfaceNormals(void) const final { return &normals; }

    AxisProjection projectOn(const Vector2f& axis, const Position& pos, const Rotation& rot) const final;
};

bool overlaps(const BoundingSurface& bsA,
              const Position& posA,
              const Rotation& rotA,
              const BoundingSurface& bsB,
              const Position& posB,
              const Rotation& rotB);
