#pragma once

#include <cassert>
#include <limits>
#include <math.h>
#include <memory>
#include <vector>

#include "Base.hpp"
#include "Geometry.hpp"
#include "Sprite.hpp"
#include "GraphicsContext.hpp"
#include "QuadTree.hpp"
#include "Sprite.hpp"
#include "Vector2D.hpp"

struct AxisProjection {
    float min = 0.f;
    float max = 0.f;
};

AxisProjection project_on(const std::vector<v2>& global_vertices, const std::vector<size_t>& indices, const v2& axis);

class SurfaceNormalSet {
private:
    std::vector<v2> normals;

public:
    SurfaceNormalSet(const std::vector<std::vector<v2>>& vertices);
    SurfaceNormalSet(const std::vector<v2>& vertices);

    SurfaceNormalSet(const std::vector<v2>& vertices,
                     const std::vector<std::vector<size_t>>& triangle_indices);

    SurfaceNormalSet(const SurfaceNormalSet& rhs);
    SurfaceNormalSet(SurfaceNormalSet&& rhs);
    SurfaceNormalSet& operator=(const SurfaceNormalSet& other);
    SurfaceNormalSet(void) {}

    void add(const v2& vec, float rotationAngle = 0.f);
    void add(const SurfaceNormalSet& rhs, float rotationAngle = 0.f);
    void add(const SurfaceNormalSet* rhs, float rotationAngle = 0.f);

    std::vector<v2>::const_iterator begin(void) const { return normals.begin(); }
    std::vector<v2>::const_iterator end(void) const { return normals.end(); }

    inline size_t size(void) const { return normals.size(); }
};

struct CollisionData {
    std::vector<std::vector<size_t>> triangle_indices;
    SurfaceNormalSet normals;
    QuadNode* node = nullptr;
    bool friendly = false;

    CollisionData(void) {}
    CollisionData(const Entity& entity);
};

bool overlaps(const CollisionData& colA, const Entity& cdA, const CollisionData& colB, const Entity& cdB);

