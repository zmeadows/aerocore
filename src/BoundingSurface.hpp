#pragma once

#include <cassert>
#include <limits>
#include <math.h>
#include <memory>
#include <vector>

#include "QuadTree.hpp"
#include "Base.hpp"
#include "Sprite.hpp"
#include "GraphicsContext.hpp"
#include "Vector2D.hpp"

struct AxisProjection {
    float min = 0.f;
    float max = 0.f;
};

AxisProjection project_on(const std::vector<v2>& vertices, const v2& axis, const v2& pos, const float rot);

class SurfaceNormalSet {
private:
    std::vector<v2> normals;

public:
    SurfaceNormalSet(const std::vector<std::vector<v2>>& vertices);
    SurfaceNormalSet(const std::vector<v2>& vertices);
    SurfaceNormalSet(const SurfaceNormalSet& rhs);
    SurfaceNormalSet(void) {}

    void add(const v2& vec, float rotationAngle = 0.f);
    void add(const SurfaceNormalSet& rhs, float rotationAngle = 0.f);
    void add(const SurfaceNormalSet* rhs, float rotationAngle = 0.f);

    std::vector<v2>::const_iterator begin(void) const { return normals.begin(); }
    std::vector<v2>::const_iterator end(void) const { return normals.end(); }

    inline size_t size(void) const { return normals.size(); }
};

//@TODO: create new struct for handling polygon decomposition elements (triangles for now)
struct CollisionData {
    SurfaceNormalSet normals;
    std::vector<std::vector<v2>> triangles;
    bool friendly = false;
    QuadNode* node = nullptr;
};


bool overlaps(const CollisionData& colA, const Entity& cdA, const CollisionData& colB, const Entity& cdB);

