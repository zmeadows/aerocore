#pragma once

#include <cassert>
#include <limits>
#include <math.h>
#include <vector>

#include <Eigen/Dense>

#include "Base.hpp"

using namespace Eigen;

struct AxisProjection {
    float min;
    float max;
};

class SurfaceNormalSet {
private:
    std::vector<Vector2f> normals;

public:
    SurfaceNormalSet(const std::vector<Vector2f>& vertices);
    SurfaceNormalSet(const SurfaceNormalSet& rhs);
    SurfaceNormalSet(void) {}

    void add(const Vector2f& vec);
    void add(const SurfaceNormalSet& rhs);
    void add(const SurfaceNormalSet* rhs);

    std::vector<Vector2f>::const_iterator begin(void) const;
    std::vector<Vector2f>::const_iterator end(void) const;

    inline size_t size(void) const { return normals.size(); }
};

struct BoundingSurface {
    virtual AxisProjection projectOn(const Vector2f& axis, const Position& pos) const = 0;
    virtual const SurfaceNormalSet* getSurfaceNormals(void) const = 0;

    virtual ~BoundingSurface() {}
};

struct PolygonalBoundingSurface final : BoundingSurface {
    std::vector<Vector2f> vertices;
    SurfaceNormalSet normals;

    PolygonalBoundingSurface(void) = delete;
    PolygonalBoundingSurface& operator=(const PolygonalBoundingSurface&) = delete;
    PolygonalBoundingSurface& operator=(PolygonalBoundingSurface&&) = delete;

    PolygonalBoundingSurface(PolygonalBoundingSurface&&) = delete;

    PolygonalBoundingSurface(std::vector<Vector2f>&& vertices_) : normals(vertices_) {
        std::swap(vertices, vertices_);
    }

    const SurfaceNormalSet* getSurfaceNormals(void) const final { return &normals; }

    AxisProjection projectOn(const Vector2f& axis, const Position& pos) const final;

    ~PolygonalBoundingSurface(void) {}
};

bool overlaps(const BoundingSurface* bsA,
              const Position* posA,
              const BoundingSurface* bsB,
              const Position* posB);

inline BoundingSurface* boundingBox(const float halfWidth, const float halfHeight) {
    return new PolygonalBoundingSurface({{-halfWidth, -halfHeight},
                                         {halfWidth, -halfHeight},
                                         {halfWidth, halfHeight},
                                         {-halfWidth, halfHeight}});
}

inline BoundingSurface* boundingSquare(const float halfWidth) { return boundingBox(halfWidth, halfWidth); }
